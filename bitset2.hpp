// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//


#ifndef BITSET2_CB_HPP
#define BITSET2_CB_HPP


#include "detail/hex_params.hpp"
#include "detail/select_base_t.hpp"
#include "detail/hash.hpp"
#include "detail/array_access.hpp"
#include "detail/array_funcs.hpp"
#include "detail/array_add.hpp"
#include "detail/array_ops.hpp"
#include "detail/bitset2_impl.hpp"

#include <bitset>
#include <climits>
#include <cstdint>
#include <array>
#include <stdexcept>
#include <utility>
#include <string>
#include <functional>
#include <type_traits>



namespace Bitset2
{



template<size_t N,
         class T= Bitset2::detail::select_base_t<N>,
         class Enabled=void> class bitset2;

template<size_t N,class T>
class bitset2<N,T,
              typename std::enable_if<   std::is_integral<T>::value
                                      && std::is_unsigned<T>::value>::type>
: public detail::bitset2_impl<N,T>
{
  enum : size_t { base_t_n_bits= detail::bitset2_impl<N,T>::base_t_n_bits };
public:
  using array_t=  typename detail::bitset2_impl<N,T>::array_t;
  using ULLONG_t= typename detail::bitset2_impl<N,T>::ULLONG_t;
  using base_t=   T;
  using detail::bitset2_impl<N,T>::n_array;

  enum : size_t { npos= detail::h_types<T>::npos };

  class reference
  {
    friend class bitset2;
    reference() noexcept {}
    reference( bitset2<N,T> *ptr, size_t bit ) noexcept
    : m_ptr( ptr )
    , m_bit( bit )
    {}
    bitset2<N,T> *m_ptr= nullptr;
    size_t       m_bit;
  public:
    ~reference() noexcept {}
    reference& operator=( bool x ) noexcept
    {
      m_ptr->set_noexcept( m_bit, x );
      return *this;
    }
    reference& operator=( reference const & r ) noexcept
    {
      m_ptr->set_noexcept( m_bit, bool( r ) );
      return *this;
    }
    reference& flip() noexcept
    {
      m_ptr->flip_noexcept( m_bit );
      return *this;
    }
    operator bool() const noexcept
    { return m_ptr->test_noexcept(m_bit); }
    bool operator~() const noexcept
    { return !bool(*this); }
  }; // class reference


  /* ------------------------------------------------------------- */
  constexpr
  bitset2() noexcept
  : detail::bitset2_impl<N,T>()
  {}

  constexpr
  bitset2( bitset2 const & ) noexcept= default;

  constexpr
  bitset2( bitset2 && ) noexcept= default;

  bitset2 &
  operator=( bitset2 const & ) noexcept= default;

  bitset2 &
  operator=( bitset2 && ) noexcept= default;

  explicit
  bitset2( const std::bitset<N> &bs ) noexcept
  : detail::bitset2_impl<N,T>( bs )
  {}

  explicit
  constexpr
  bitset2( ULLONG_t v ) noexcept
  : detail::bitset2_impl<N,T>( v )
  {}

  template<size_t n>
  explicit
  constexpr
  bitset2( typename
           detail::h_types<T>::template array_t<n> const & value
         ) noexcept : detail::bitset2_impl<N,T>( value )
  {}

  template< class CharT, class Traits, class Alloc >
  explicit
  bitset2( std::basic_string<CharT,Traits,Alloc> const
                    & str,
           typename std::basic_string<CharT,Traits,Alloc>::size_type
                    pos = 0,
           typename std::basic_string<CharT,Traits,Alloc>::size_type
                    n = std::basic_string<CharT,Traits,Alloc>::npos,
           CharT    zero= CharT('0'),
           CharT    one=  CharT('1') )
  : detail::bitset2_impl<N,T>( str, pos, n, zero, one )
  {}


  template< class CharT >
  explicit
  bitset2( const CharT        *str,
           typename std::basic_string<CharT>::size_type
                              n=     std::basic_string<CharT>::npos,
           CharT              zero=  CharT('0'),
           CharT              one=   CharT('1') )
  : detail::bitset2_impl<N,T>( n == std::basic_string<CharT>::npos
                               ? std::basic_string<CharT>( str )
                               : std::basic_string<CharT>( str, n ),
                               0, n, zero, one )
  {}
  /* ------------------------------------------------------------- */


  //****************************************************

  /// Bitwise NOT
  constexpr
  bitset2
  operator~() const noexcept
  { return bitset2(detail::array_ops<N,T>( 0 ).flip(this->data())); }

  constexpr
  bool
  operator[]( size_t bit ) const noexcept
  { return detail::bitset2_impl<N,T>::operator[]( bit ); }

  reference
  operator[]( size_t bit ) noexcept
  { return reference( this, bit ); }

  bitset2 &
  operator<<=( size_t n_shift ) noexcept
  {
    detail::array_ops<N,T>( n_shift ).shift_left_assgn( this->get_data() );
    return *this;
  }

  bitset2 &
  operator>>=( size_t n_shift ) noexcept
  {
    detail::array_ops<N,T>( n_shift ).shift_right_assgn( this->get_data() );
    return *this;
  }

  bitset2 &
  rotate_left( size_t n_rot ) noexcept
  {
    this->get_data()= detail::array_ops<N,T>(n_rot).rotate_left( this->data() );
    return *this;
  }

  bitset2 &
  rotate_right( size_t n_rot ) noexcept
  {
    this->get_data()= detail::array_ops<N,T>( N - ( n_rot % N ) )
                                        .rotate_left( this->data() );
    return *this;
  }

  bitset2 &
  reverse() noexcept
  {
    this->get_data()= detail::array_ops<N,T>( 0 ).reverse( this->data() );
    return *this;
  }

  bitset2 &
  operator+=( bitset2 const &bs2 ) noexcept
  {
    detail::array_add<N,T>().add_assgn(this->get_data(), bs2.data());
    return *this;
  }

  bitset2 &
  operator++() noexcept
  {
    detail::array_ops<N,T>(0).increment( this->get_data() );
    return *this;
  }

  bitset2
  operator++(int) noexcept
  {
    bitset2 tmp( *this );
    operator++();
    return tmp;
  }

  bitset2 &
  operator--() noexcept
  {
    detail::array_ops<N,T>(0).decrement( this->get_data() );
    return *this;
  }

  bitset2
  operator--(int) noexcept
  {
    bitset2 tmp( *this );
    operator--();
    return tmp;
  }

  bitset2 &
  operator|=( bitset2 const & v2 ) noexcept
  {
    detail::array_funcs<bitset2::n_array,T>()
             .bitwise_or_assgn(this->get_data(), v2.data() );
    return *this;
  }

  bitset2 &
  operator&=( bitset2 const & v2 ) noexcept
  {
    detail::array_funcs<bitset2::n_array,T>()
              .bitwise_and_assgn( this->get_data(), v2.data() );
    return *this;
  }

  bitset2 &
  operator^=( bitset2 const & v2 ) noexcept
  {
    detail::array_funcs<bitset2::n_array,T>()
              .bitwise_xor_assgn( this->get_data(), v2.data() );
    return *this;
  }

  /// Computes the set difference, i.e. *this &= ~v2
  bitset2 &
  difference( bitset2 const & v2 ) noexcept
  {
    detail::array_funcs<bitset2::n_array,T>()
              .bitwise_setdiff_assgn( this->get_data(), v2.data() );
    return *this;
  }

  bitset2 &
  set() noexcept
  { detail::bitset2_impl<N,T>::set(); return *this; }

  bitset2 &
  set( size_t bit, bool value= true )
  { detail::bitset2_impl<N,T>::set( bit, value ); return *this; }

  bitset2 &
  reset() noexcept
  { detail::bitset2_impl<N,T>::reset(); return *this; }

  bitset2 &
  reset( size_t bit )
  {
    if( bit >= N  ) throw std::out_of_range( "bitset2: reset out of range" );
   return set( bit, false );
 }

 /// \brief Sets the specified bit if value==true,
 /// clears it otherwise. Returns the previous state of the bit.
 bool
 test_set( size_t bit, bool value= true )
 { return detail::bitset2_impl<N,T>::test_set( bit, value ); }

  bitset2 &
  flip() noexcept
  { detail::bitset2_impl<N,T>::flip(); return *this; }

  bitset2 &
  flip( size_t bit )
  { detail::bitset2_impl<N,T>::flip( bit ); return *this; }

  constexpr std::size_t size() const noexcept { return N; }

  template<class CharT = char,
           class Traits = std::char_traits<CharT>,
           class Allocator = std::allocator<CharT> >
  std::basic_string<CharT,Traits,Allocator>
  to_string( CharT zero = CharT('0'), CharT one = CharT('1') ) const
  {
    std::basic_string<CharT,Traits,Allocator> ret_val;
    ret_val.reserve( N );
    for( size_t ct= N; ct > 0; )
    {
      --ct;
      ret_val += this->operator[]( ct ) ? one : zero;
    }
    return ret_val;
  } // to_string

  template<class CharT = char,
           class Traits = std::char_traits<CharT>,
           class Allocator = std::allocator<CharT>,
           typename std::enable_if<base_t_n_bits % 4 == 0>::type* = nullptr >
  std::basic_string<CharT,Traits,Allocator>
  to_hex_string( hex_params<CharT,Traits,Allocator> const &params=
                 hex_params<CharT,Traits,Allocator>{} ) const
  {
    using arr_acc=  detail::array_access<N,T>;
    arr_acc         a_a;
    constexpr auto  div_four= arr_acc::div_four;
    constexpr auto  mod_four= arr_acc::mod_four;
    constexpr auto  n_char= div_four + ( mod_four > 0 );

    auto const      zeroCh= params.zeroCh;
    auto const      aCh=    params.aCh;

    std::basic_string<CharT,Traits,Allocator> ret_val;
    ret_val.reserve( n_char + params.prefix.size() );
    ret_val= params.prefix;
    size_t ct= n_char;
    if( !params.leadingZeroes )
    {
      for( ; ct > 0; --ct )
      {
        auto  const val= a_a.get_four_bits( this->data(), 4 * ct - 1 );
        if( val != 0 ) break;
      }
    }
    if( ct == 0 && params.nonEmpty ) ret_val += zeroCh;
    for( ; ct > 0; --ct )
    {
      auto  const val= a_a.get_four_bits( this->data(), 4 * ct - 1 );
      CharT const c=
            ( val < 10 ) ? ( zeroCh + val ) : ( aCh + ( val - 10 ) );
      ret_val += c;
    }
    return ret_val;
  } // to_hex_string

}; // class bitset2


template<size_t N, class T>
constexpr
bitset2<N,T>
rotate_left( bitset2<N,T> const & bs, size_t n_rot ) noexcept
{
  return
    bitset2<N,T>( detail::array_ops<N,T>( n_rot ).rotate_left( bs.data() ) );
}


template<size_t N, class T>
constexpr
bitset2<N,T>
rotate_right( bitset2<N,T> const & bs, size_t n_rot ) noexcept
{
  return
    bitset2<N,T>( detail::array_ops<N,T>( N - ( n_rot % N ) ).
                                               rotate_left( bs.data() ) );
}


/// Computes the set difference, i.e. bs1 & ~bs2
template<size_t N, class T>
constexpr
bitset2<N,T>
difference( bitset2<N,T> const & bs1, bitset2<N,T> const & bs2 ) noexcept
{
  return
    bitset2<N,T>( detail::array_funcs<bitset2<N,T>::n_array,T>()
                            .bitwise_setdiff( bs1.data(), bs2.data() ) );
}


/// Returns bs with bits reversed
template<size_t N, class T>
constexpr
bitset2<N,T>
reverse( bitset2<N,T> const & bs ) noexcept
{ return bitset2<N,T>( detail::array_ops<N,T>( 0 ).reverse( bs.data() ) ); }


//TODO: Treat different base_t
/// Converts an M-bit bitset2 to an N-bit bitset2.
template<size_t N,size_t M, class T>
constexpr
bitset2<N,T>
convert_to( bitset2<M,T> const & bs ) noexcept
{ return bitset2<N,T>( bs.data() ); }


/// \brief Returns true if f returns true for each pair
/// of base_t=T values in bs1 and bs2. f should be a binary function
/// taking two base_t values and returning bool.
/// zip_fold_and does short circuit if possible.
template<size_t N, class F, class T>
constexpr
bool
zip_fold_and( bitset2<N,T> const & bs1, bitset2<N,T> const & bs2,
              F f ) noexcept(noexcept( f( T(0), T(0) ) ))
{
  return
    detail::array_funcs<bitset2<N,T>::n_array,T>().zip_fold_and(bs1.data(),
                                                                bs2.data(), f);
}


/// \brief Returns true if f returns true for at least one pair
/// of base_t=T values in bs1 and bs2. f should be a binary function
/// taking two base_t values and returning bool.
/// zip_fold_or does short circuit if possible.
template<size_t N, class F,class T>
constexpr
bool
zip_fold_or( bitset2<N,T> const & bs1, bitset2<N,T> const & bs2,
             F f ) noexcept(noexcept( f( T(0), T(0) ) ))
{
  return
    detail::array_funcs<bitset2<N,T>::n_array,T>().zip_fold_or( bs1.data(),
                                                                bs2.data(), f );
}


} // namespace Bitset2




/// Stream output
template <class CharT, class Traits, size_t N,class T>
std::basic_ostream<CharT, Traits>&
operator<<( std::basic_ostream<CharT, Traits> & os,
            Bitset2::bitset2<N,T> const & x )
{
  for( size_t ct= N; ct > 0; )
  {
     --ct;
     os << ( x[ct] ? "1" : "0" );
  }
  return os;
}

/// Stream input
template <class CharT, class Traits, size_t N,class T>
std::basic_istream<CharT, Traits>&
operator>>( std::basic_istream<CharT, Traits> & is,
            Bitset2::bitset2<N,T> & x )
{
  std::bitset<N>  bs;
  is >> bs;
  x= Bitset2::bitset2<N,T>( bs );
  return is;
}



/// Shift left
template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
operator<<( Bitset2::bitset2<N,T> const & bs, size_t n_shift ) noexcept
{
  return
    Bitset2::bitset2<N,T>( Bitset2::detail::array_ops<N,T>( n_shift )
                                                 .shift_left( bs.data() ) );
}


/// Shift right
template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
operator>>( Bitset2::bitset2<N,T> const & bs, size_t n_shift ) noexcept
{
  return
    Bitset2::bitset2<N,T>( Bitset2::detail::array_ops<N,T>( n_shift )
                                                 .shift_right( bs.data() ) );
}


template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
operator|( Bitset2::bitset2<N,T> const & bs1,
           Bitset2::bitset2<N,T> const & bs2 ) noexcept
{
  return
    Bitset2::bitset2<N,T>(
      Bitset2::detail::array_funcs<Bitset2::bitset2<N,T>::n_array,T>()
                                    .bitwise_or( bs1.data(), bs2.data() ) );
}


template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
operator&( Bitset2::bitset2<N,T> const & bs1,
           Bitset2::bitset2<N,T> const & bs2 ) noexcept
{
  return
    Bitset2::bitset2<N,T>(
      Bitset2::detail::array_funcs<Bitset2::bitset2<N,T>::n_array,T>()
                                  .bitwise_and( bs1.data(), bs2.data() ) );
}


template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
operator^( Bitset2::bitset2<N,T> const & bs1,
           Bitset2::bitset2<N,T> const & bs2 ) noexcept
{
  return
    Bitset2::bitset2<N,T>(
      Bitset2::detail::array_funcs<Bitset2::bitset2<N,T>::n_array,T>()
                                 .bitwise_xor( bs1.data(), bs2.data() ) );
}


template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
operator+( Bitset2::bitset2<N,T> const & bs1,
           Bitset2::bitset2<N,T> const & bs2 ) noexcept
{
  return
    Bitset2::bitset2<N,T>(
      Bitset2::detail::array_add<N,T>().add( bs1.data(), bs2.data() ) );
}


namespace std
{
  template<size_t N,class T>
  struct hash<Bitset2::bitset2<N,T> >
  {
  private:
    enum : size_t
    { n_array= Bitset2::detail::bitset2_impl<N,T>::n_array };

    Bitset2::detail::hash_impl<n_array,T>   m_func;

  public:
    using argument_type= Bitset2::bitset2<N,T>;
    using result_type=
          typename Bitset2::detail::hash_impl<n_array,T>::result_type;

    result_type operator()( argument_type const& bs ) const
    { return m_func( bs.data() ); }
  }; // struct hash

} // namespace std


#endif // BITSET2_CB_HPP

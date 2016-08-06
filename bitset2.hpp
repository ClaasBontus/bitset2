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


#include "detail/hash.hpp"
#include "detail/bit_chars.hpp"
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



template<size_t N>
class bitset2
: public detail::bitset2_impl<N>
{
  static const auto   ullong_bits= detail::bitset2_impl<N>::ullong_bits;
public:
  using array_t= typename detail::bitset2_impl<N>::array_t;
  using ULLONG=  typename detail::bitset2_impl<N>::ULLONG;
  using detail::bitset2_impl<N>::n_array;

  class reference
  {
    friend class bitset2;
    reference() noexcept {}
    reference( bitset2<N> *ptr, size_t bit ) noexcept
    : m_ptr( ptr )
    , m_bit( bit )
    {}
    bitset2<N> *m_ptr= nullptr;
    size_t      m_bit;
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
    { return (*m_ptr)[m_bit]; }
    bool operator~() const noexcept
    { return !bool(*this); }
  }; // class reference

  constexpr
  bitset2() noexcept
  : detail::bitset2_impl<N>()
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
  : detail::bitset2_impl<N>( bs )
  {}

  explicit
  constexpr
  bitset2( ULLONG v ) noexcept
  : detail::bitset2_impl<N>( v )
  {}

  template<size_t n>
  explicit
  constexpr
  bitset2( detail::h_types::array_t<n> const & value ) noexcept
  : detail::bitset2_impl<N>( value )
  {}

  template< class CharT, class Traits, class Alloc >
  explicit
  bitset2( std::basic_string<CharT,Traits,Alloc> const               &str,
           typename std::basic_string<CharT,Traits,Alloc>::size_type pos = 0,
           typename std::basic_string<CharT,Traits,Alloc>::size_type n =
                                  std::basic_string<CharT,Traits,Alloc>::npos,
           CharT   zero= CharT('0'),
           CharT   one=  CharT('1') )
  : detail::bitset2_impl<N>( str, pos, n, zero, one )
  {}


  template< class CharT >
  explicit
  bitset2( const CharT                                    *str,
           typename std::basic_string<CharT>::size_type   n =
                    std::basic_string<CharT>::npos,
           CharT                                          zero=  CharT('0'),
           CharT                                          one=   CharT('1') )
  : detail::bitset2_impl<N>( n == std::basic_string<CharT>::npos
                             ? std::basic_string<CharT>( str )
                             : std::basic_string<CharT>( str, n ),
                             0, n, zero, one )
  {}


  //****************************************************

  /// Bitwise NOT
  constexpr
  bitset2
  operator~() const noexcept
  {
    return bitset2( detail::array_ops<N>( 0 ).flip( this->data() ) );
  }

  constexpr
  bool
  operator[]( size_t bit ) const noexcept
  { return detail::bitset2_impl<N>::operator[]( bit ); }

  reference
  operator[]( size_t bit ) noexcept
  { return reference( this, bit ); }

  bitset2 &
  operator<<=( size_t n_shift ) noexcept
  {
    this->get_data()= detail::array_ops<N>( n_shift ).shift_left( this->data());
    return *this;
  }

  bitset2 &
  operator>>=( size_t n_shift ) noexcept
  {
    this->get_data()= detail::array_ops<N>( n_shift ).shift_right(this->data());
    return *this;
  }

  bitset2 &
  rotate_left( size_t n_rot ) noexcept
  {
    this->get_data()= detail::array_ops<N>( n_rot ).rot_left( this->data() );
    return *this;
  }

  bitset2 &
  rotate_right( size_t n_rot ) noexcept
  {
    this->get_data()= detail::array_ops<N>( n_rot ).rot_right( this->data() );
    return *this;
  }

  bitset2 &
  operator+=( bitset2 const &bs2 ) noexcept
  {
    this->get_data()= detail::array_add<N>().add( this->data(), bs2.data() );
    return *this;
  }

  bitset2 &
  operator++() noexcept
  {
    detail::array_ops<N>(0).increment( this->get_data() );
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
    detail::array_ops<N>(0).decrement( this->get_data() );
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
    this->get_data()=
          detail::array_funcs<bitset2::n_array>().bitwise_or( this->data(),
                                                           v2.data() );
    return *this;
  }

  bitset2 &
  operator&=( bitset2 const & v2 ) noexcept
  {
    this->get_data()=
          detail::array_funcs<bitset2::n_array>().bitwise_and( this->data(),
                                                            v2.data() );
    return *this;
  }

  bitset2 &
  operator^=( bitset2 const & v2 ) noexcept
  {
    this->get_data()=
          detail::array_funcs<bitset2::n_array>().bitwise_xor( this->data(),
                                                            v2.data() );
    return *this;
  }

  bitset2 &
  set() noexcept
  { detail::bitset2_impl<N>::set(); return *this; }

  bitset2 &
  set( size_t bit, bool value= true )
  { detail::bitset2_impl<N>::set( bit, value ); return *this; }

  bitset2 &
  reset() noexcept
  { detail::bitset2_impl<N>::reset(); return *this; }

  bitset2 &
  reset( size_t bit )
  {
    if( bit >= N  )
      throw std::out_of_range( "bitset2: Resetting of bit out of range" );
   return set( bit, false );
 }

  bitset2 &
  flip() noexcept
  { detail::bitset2_impl<N>::flip(); return *this; }

  bitset2 &
  flip( size_t bit )
  { detail::bitset2_impl<N>::flip( bit ); return *this; }

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
           typename std::enable_if<ullong_bits % 4 == 0>::type* = nullptr >
  std::basic_string<CharT,Traits,Allocator>
  to_hex_string() const
  {
    std::basic_string<CharT,Traits,Allocator> ret_val;
    using arr_acc=  detail::array_access<N>;
    arr_acc         a_a;
    constexpr auto  div_four= arr_acc::div_four;
    constexpr auto  mod_four= arr_acc::mod_four;
    constexpr auto  n_char= div_four + ( mod_four > 0 );
    ret_val.reserve( n_char );
    for( size_t ct= n_char; ct > 0; --ct )
    {
      auto const val= a_a.get_four_bits( this->data(), 4 * ct - 1 );
      char const c= ( val < 10 ) ? ( '0' + val ) : ( 'a' + ( val - 10 ) );
      ret_val += c;
    }
    return ret_val;
  } // to_hex_string

}; // class bitset2


template<size_t N>
constexpr
bitset2<N>
rotate_left( bitset2<N> const & bs, size_t n_rot ) noexcept
{
  return
    bitset2<N>( detail::array_ops<N>( n_rot ).rot_left( bs.data() ) );
}


template<size_t N>
constexpr
bitset2<N>
rotate_right( bitset2<N> const & bs, size_t n_rot ) noexcept
{
  return
    bitset2<N>( detail::array_ops<N>( n_rot ).rot_right( bs.data() ) );
}


/// Converts an M-bit bitset2 to an N-bit bitset2.
template<size_t N,size_t M>
constexpr
bitset2<N>
convert_to( bitset2<M> const & bs ) noexcept
{
  return bitset2<N>( bs.data() );
}


} // namespace Bitset2




/// Stream output
template <class CharT, class Traits, size_t N>
std::basic_ostream<CharT, Traits>&
operator<<( std::basic_ostream<CharT, Traits> & os,
            Bitset2::bitset2<N> const & x )
{
  for( size_t ct= N; ct > 0; )
  {
     --ct;
     os << ( x[ct] ? "1" : "0" );
  }
  return os;
}

/// Stream input
template <class CharT, class Traits, size_t N>
std::basic_istream<CharT, Traits>&
operator>>( std::basic_istream<CharT, Traits> & is,
            Bitset2::bitset2<N> & x )
{
  std::bitset<N>  bs;
  is >> bs;
  x= Bitset2::bitset2<N>( bs );
  return is;
}



/// Shift left
template<size_t N>
constexpr
Bitset2::bitset2<N>
operator<<( Bitset2::bitset2<N> const & bs, size_t n_shift ) noexcept
{
  return
    Bitset2::bitset2<N>( Bitset2::detail::array_ops<N>( n_shift )
                                                 .shift_left( bs.data() ) );
}


/// Shift right
template<size_t N>
constexpr
Bitset2::bitset2<N>
operator>>( Bitset2::bitset2<N> const & bs, size_t n_shift ) noexcept
{
  return
    Bitset2::bitset2<N>( Bitset2::detail::array_ops<N>( n_shift )
                                                 .shift_right( bs.data() ) );
}


template<size_t N>
constexpr
Bitset2::bitset2<N>
operator|( Bitset2::bitset2<N> const & bs1,
           Bitset2::bitset2<N> const & bs2 ) noexcept
{
  return
    Bitset2::bitset2<N>(
      Bitset2::detail::array_funcs<Bitset2::bitset2<N>::n_array>()
                                    .bitwise_or( bs1.data(), bs2.data() ) );
}


template<size_t N>
constexpr
Bitset2::bitset2<N>
operator&( Bitset2::bitset2<N> const & bs1,
           Bitset2::bitset2<N> const & bs2 ) noexcept
{
  return
    Bitset2::bitset2<N>(
      Bitset2::detail::array_funcs<Bitset2::bitset2<N>::n_array>()
                                  .bitwise_and( bs1.data(), bs2.data() ) );
}


template<size_t N>
constexpr
Bitset2::bitset2<N>
operator^( Bitset2::bitset2<N> const & bs1,
           Bitset2::bitset2<N> const & bs2 ) noexcept
{
  return
    Bitset2::bitset2<N>(
      Bitset2::detail::array_funcs<Bitset2::bitset2<N>::n_array>()
                                 .bitwise_xor( bs1.data(), bs2.data() ) );
}


template<size_t N>
constexpr
Bitset2::bitset2<N>
operator+( Bitset2::bitset2<N> const & bs1,
           Bitset2::bitset2<N> const & bs2 ) noexcept
{
  return
    Bitset2::bitset2<N>(
      Bitset2::detail::array_add<N>().add( bs1.data(), bs2.data() ) );
}


namespace std
{
    template<size_t N>
    struct hash<Bitset2::bitset2<N> >
    {
      using argument_type= Bitset2::bitset2<N>;
      using result_type=   std::size_t;

      result_type operator()( argument_type const& bs ) const
      {
//        return m_func( std::bitset<N>( bs ) );
        return m_func( bs.data() );
      }
    private:
      enum : size_t
      { n_array= Bitset2::detail::bitset2_impl<N>::n_array };

      Bitset2::detail::hash_impl<n_array>   m_func;
//       std::hash<std::bitset<N> >  m_func;
    }; // struct hash
} // namespace std


#endif // BITSET2_CB_HPP

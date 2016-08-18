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

#ifndef BITSET2_IMPL_CB_HPP
#define BITSET2_IMPL_CB_HPP

#include "bit_chars.hpp"
#include "array_add.hpp"
#include <bitset>


namespace Bitset2
{
namespace detail
{

template<size_t N>
class bitset2_impl
{
  using b_chars=                     bit_chars<N>;

public:
  enum : size_t { n_array= b_chars::n_array };
  using ULONG=                       typename b_chars::ULONG;
  using ULLONG=                      typename b_chars::ULLONG;
  using array_t=                     h_types::array_t<n_array>;

protected:
  static const auto n_ullong=        b_chars::n_ullong;
  static const auto ulong_bits=      b_chars::ulong_bits;
  static const auto ullong_bits=     b_chars::ullong_bits;
  static const auto ulong_max=       b_chars::ulong_max;
  static const auto low_bit_pattern= b_chars::low_bit_pattern;
  static const auto hgh_bit_pattern= b_chars::hgh_bit_pattern;

  constexpr
  bitset2_impl() noexcept
  {}

  constexpr
  bitset2_impl( bitset2_impl const & ) noexcept= default;

  constexpr
  bitset2_impl( bitset2_impl && ) noexcept= default;

  bitset2_impl &
  operator=( bitset2_impl const & ) noexcept= default;

  bitset2_impl &
  operator=( bitset2_impl && ) noexcept= default;

  explicit
  constexpr
  bitset2_impl( ULLONG v ) noexcept
  : m_value{ { v & low_bit_pattern } }
  {}

  template<size_t n>
  explicit
  constexpr
  bitset2_impl( h_types::array_t<n> const & value ) noexcept
  : m_value( detail::array_funcs<n_array>().copy_and_map(hgh_bit_pattern,value))
  {}

  explicit
  bitset2_impl( const std::bitset<N> &bs ) noexcept
  {
    if( N == 0 ) return;
    if( n_ullong == 1 )
    {
      m_value[0]= bs.to_ullong();
      return;
    }

    size_t offset= 0;
    for( ULLONG ct= 0; ct < n_ullong; ++ct )
    {
      ULLONG  val= 0ull;
      auto const bit_limit=
      ( ct < n_ullong - 1 ) ? ullong_bits : N - offset;
      for( size_t bit_ct= 0; bit_ct < bit_limit; ++bit_ct )
      {
        auto const test_bit= offset + bit_limit - bit_ct - 1;
        val <<= 1;
        if( bs.test( test_bit ) ) val |= 1ull;
      } // for bit_ct
      m_value[ct]= val;
      offset += ullong_bits;
    } // for ct
  } // bitset2_impl( const std::bitset<N> &bs )


  template< class CharT, class Traits, class Alloc >
  explicit
  bitset2_impl( std::basic_string<CharT,Traits,Alloc> const      &str,
                typename
                std::basic_string<CharT,Traits,Alloc>::size_type pos,
                typename
                std::basic_string<CharT,Traits,Alloc>::size_type n,
                CharT                                            zero,
                CharT                                            one )
  {
    auto const str_sz= str.size();
    if( pos > str_sz )
            throw std::out_of_range( "bitset2: String submitted to "
                                     "constructor smaller than pos" );
    auto const n_bits= std::min( N, std::min( n, str_sz - pos ) );

    for( size_t bit_ct= 0; bit_ct < n_bits; ++bit_ct )
    {
      auto const chr= str[bit_ct+pos];
      if(       Traits::eq( one,  chr ) ) set( n_bits - bit_ct - 1 );
      else if( !Traits::eq( zero, chr ) )
              throw std::invalid_argument( "bitset2: Invalid argument in "
                                           "string submitted to constructor" );
    } // for bit_ct
  }


  //**********************************************************


  array_t &
  get_data() noexcept
  { return m_value; }

  constexpr
  bool
  operator[]( size_t bit ) const noexcept
  { return m_value[bit / ullong_bits] & ( 1ull << ( bit % ullong_bits ) ); }

  bitset2_impl &
  set( size_t bit, bool value= true )
  {
    if( bit >= N  )
            throw std::out_of_range( "bitset2: Setting of bit out of range" );
    set_noexcept( bit, value );
    return *this;
  } // set

  bitset2_impl &
  set() noexcept
  {
    if( N > 0 )
    {
      size_t c= 0;
      for( ; c < n_ullong - 1; ++c ) m_value[c]= ~(0ull);
      m_value[c]= hgh_bit_pattern;
    }
    return *this;
  } // set

  bitset2_impl &
  reset() noexcept
  {
    for( size_t c= 0; c < n_array; ++c ) m_value[c]= 0ull;
    return *this;
  }

  bitset2_impl &
  flip_noexcept( size_t bit ) noexcept
  {
    m_value[bit / ullong_bits] ^= ( 1ull << ( bit % ullong_bits ) );
    return *this;
  }

  bitset2_impl &
  flip( size_t bit )
  {
    if( bit >= N  )
            throw std::out_of_range( "bitset2: Flipping of bit out of range" );
    return flip_noexcept( bit );
  } // flip

  bitset2_impl &
  flip() noexcept
  {
    if( N > 0 )
    {
      size_t c= 0;
      for( ; c < n_ullong - 1; ++c ) m_value[c] ^= ~(0ull);
      m_value[c] ^= hgh_bit_pattern;
    }
    return *this;
  } // flip

public:
  constexpr
  array_t const &
  data() const noexcept
  { return m_value; }

  constexpr
  ULONG
  to_ulong() const
  {
    return  ( N == 0 ) ? 0ul
            : ( N > ulong_bits
                && ( m_value[0] > ulong_max
                     || count() != count_bits(m_value[0]) ) )
              ? throw std::overflow_error( "Cannot convert bitset2 "
                                           "to unsigned long" )
              : ULONG( m_value[0] );
  } // to_ulong

  constexpr
  ULLONG
  to_ullong() const
  {
    return  ( N == 0 ) ? 0ull
            : ( n_ullong > 1 && count() != count_bits(m_value[0]) )
              ? throw std::overflow_error( "Cannot convert bitset2 "
                                           "to unsigned long long" )
              : m_value[0];
  } // to_ullong

  constexpr
  bool
  test( size_t bit ) const
  {
    return  ( bit >= N )
            ? throw std::out_of_range( "bitset2: Testing of bit out of range" )
            : operator[]( bit );
  }

  void
  set_noexcept( size_t bit, bool value= true ) noexcept
  {
    if( value ) m_value[bit / ullong_bits] |=  (1ull << ( bit % ullong_bits ));
    else        m_value[bit / ullong_bits] &= ~(1ull << ( bit % ullong_bits ));
  }

  constexpr
  bool
  none() const noexcept
  { return detail::array_funcs<n_array>().none( m_value ); }

  constexpr
  bool
  any() const noexcept
  { return ( N > 0 ) && !none(); }

  constexpr
  bool
  all() const noexcept
  { return ( N > 0 ) && detail::array_ops<N>( 0 ).all( m_value ); }

  constexpr
  size_t
  count() const noexcept
  { return detail::array_funcs<n_array>().count( m_value ); }


  constexpr
  bool
  operator==( bitset2_impl const &v2 ) const noexcept
  { return detail::array_funcs<n_array>().equal( m_value, v2.m_value ); }

  constexpr
  bool
  operator!=( bitset2_impl const &v2 ) const noexcept
  { return !( *this == v2 ); }

  constexpr
  bool
  operator<( bitset2_impl const &v2 ) const noexcept
  { return detail::array_funcs<n_array>().less_than(m_value, v2.m_value); }

  constexpr
  bool
  operator<=( bitset2_impl const &v2 ) const noexcept
  { return ! ( *this > v2 ); }

  constexpr
  bool
  operator>( bitset2_impl const &v2 ) const noexcept
  { return detail::array_funcs<n_array>().less_than(v2.m_value, m_value); }

  constexpr
  bool
  operator>=( bitset2_impl const &v2 ) const noexcept
  { return ! ( *this < v2 ); }

  explicit
  operator std::bitset<N>() const
  {
    using b_t= std::bitset<N>;
    if( N == 0 ) return b_t{};
    if( n_ullong == 1 ) return b_t( to_ullong() );

    b_t  ret_val;
    for( size_t ct= 0; ct < N; ++ct ) ret_val[ct]= operator[](ct);

    return ret_val;
  }
private:
  array_t   m_value= gen_empty_array<n_array>();
}; // class bitset2_impl


} // namespace detail
} // namespace Bitset2



#endif // BITSET2_IMPL_CB_HPP

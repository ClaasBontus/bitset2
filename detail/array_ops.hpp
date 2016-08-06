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

#ifndef BITSET2_ARRAY_OPS_CB_HPP
#define BITSET2_ARRAY_OPS_CB_HPP

#include "bit_chars.hpp"


namespace Bitset2
{
namespace detail
{

  template<size_t N>
  struct array_ops
  {
    using b_chars=                     bit_chars<N>;
    using ULLONG=                      h_types::ULLONG;
    enum : size_t {   ullong_bits=     b_chars::ullong_bits
                    , n_ullong=        b_chars::n_ullong
                    , n_array=         b_chars::n_array
                  };
    enum : ULLONG {   hgh_bit_pattern= b_chars::hgh_bit_pattern };
    using array_t=                     h_types::array_t<n_array>;
    using zero_array_t=                h_types::array_t<0>;

    constexpr
    array_ops( size_t n_shift ) noexcept
    : m_n_shift_mod( n_shift % N )
    , m_shft_div( n_shift / ullong_bits )
    , m_shft_mod( n_shift % ullong_bits )
    , m_shft_leftright_shift( ullong_bits-(n_shift % ullong_bits) )
    , m_shft_left_pattern( ull_left_shift( ~(0ull), 
                                           ullong_bits-(n_shift % ullong_bits)))
    , m_shft_right_pattern( ull_right_shift( ~(0ull),
                                             ullong_bits-(n_shift%ullong_bits)))
    {}

    constexpr
    array_t
    shift_left( array_t const &arr ) const noexcept
    { return shift_left_impl( arr, std::make_index_sequence<n_array>() ); }

    constexpr
    array_t
    shift_right( array_t const &arr ) const noexcept
    { return shift_right_impl( arr, std::make_index_sequence<n_array>() ); }

    constexpr
    array_t
    rot_left( array_t const &arr ) const noexcept
    {
      return
        array_funcs<n_array>()
            .bitwise_or( array_ops<N>(     m_n_shift_mod ).shift_left(  arr ),
                         array_ops<N>( N - m_n_shift_mod ).shift_right( arr ) );
    }

    constexpr
    array_t
    rot_right( array_t const &arr ) const noexcept
    {
      return
        array_funcs<n_array>()
            .bitwise_or( array_ops<N>(     m_n_shift_mod ).shift_right( arr ),
                         array_ops<N>( N - m_n_shift_mod ).shift_left(  arr ) );
    }

    constexpr
    array_t
    flip( array_t const &arr ) const noexcept
    { return flip_impl( arr, std::make_index_sequence<n_array>() ); }

    constexpr
    bool
    all( array_t const &arr ) const noexcept
    { return ( N > 0 ) && all_impl( n_ullong - 1, arr ); }


    void
    increment( array_t &arr ) const noexcept
    {
      if( N == 0 ) return;

      size_t  c= 0;
      for( ; c + 1 < n_ullong; ++c )
      {
        if( ( ++arr[c] ) != 0ull ) return;
      }
      ++arr[c];
      arr[c] &= hgh_bit_pattern;
    } // increment


    void
    decrement( array_t &arr ) const noexcept
    {
      if( N == 0 ) return;

      size_t  c= 0;
      for( ; c + 1 < n_ullong; ++c )
      {
        if( ( arr[c]-- ) != 0ull ) return;
      }
      --arr[c];
      arr[c] &= hgh_bit_pattern;

    } // decrement


    //****************************************************


    constexpr
    bool
    all_impl( size_t idx, array_t const &arr ) const noexcept
    {
      return    h_all( idx, arr )
             && ( ( idx == 0 ) ? true : all_impl( idx - 1, arr ) );
    }

    constexpr
    bool
    h_all( size_t idx, array_t const &arr ) const noexcept
    {
      return ( idx + 1 ==  n_ullong ) ? ( arr[idx] == hgh_bit_pattern )
                                      : ( arr[idx] == (~(0ull) ) );
    }


    template<size_t ... S>
    constexpr
    array_t
    shift_left_impl( array_t const &arr,
                     std::index_sequence<S...> ) const noexcept
    { return {{ h_shift_left( S, arr )... }};  }

    constexpr
    ULLONG
    h_shift_left( size_t idx, array_t const &arr ) const noexcept
    {
      return   h_shift_left1( idx, arr )
             & ( ( idx+1 == n_ullong ) ?  hgh_bit_pattern : (~(0ull)) );
    }

    constexpr
    ULLONG
    h_shift_left1( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx >= n_ullong || idx < m_shft_div ) ? 0ull
             : ( ull_left_shift( arr[idx-m_shft_div], m_shft_mod )
               | h_shift_left_rem( idx - m_shft_div, arr ) );
    }

    constexpr
    ULLONG
    h_shift_left_rem( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx == 0 ) ? 0ull
             : ull_right_shift( arr[idx-1] & m_shft_left_pattern,
                                m_shft_leftright_shift );
    }


    template<size_t ... S>
    constexpr
    array_t
    shift_right_impl( array_t const &arr,
                      std::index_sequence<S...> ) const noexcept
    { return {{ h_shift_right( S, arr )... }};  }

    constexpr
    ULLONG
    h_shift_right( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx + m_shft_div >= n_ullong ) ? 0ull
             : ( ull_right_shift( arr[idx+m_shft_div], m_shft_mod )
               | h_shift_right_rem( idx + m_shft_div, arr ) );
    }

    constexpr
    ULLONG
    h_shift_right_rem( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx + 1 >= n_ullong ) ? 0ull
             : ull_left_shift( arr[idx+1] & m_shft_right_pattern,
                               m_shft_leftright_shift );
    }


    template<size_t ... S>
    constexpr
    array_t
    flip_impl( array_t const &arr, std::index_sequence<S...> ) const noexcept
    { return {{ h_flip( S, arr )... }}; }

    constexpr
    ULLONG
    h_flip( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx >= n_ullong ) ? 0ull
             :   ( ~arr[idx] )
               & ( (idx+1 == n_ullong) ? hgh_bit_pattern : ~(0ull) );
    }

    size_t const     m_n_shift_mod;
    size_t const     m_shft_div;
    size_t const     m_shft_mod;
    size_t const     m_shft_leftright_shift;
    ULLONG const     m_shft_left_pattern;
    ULLONG const     m_shft_right_pattern;

  }; // struct array_ops

} // namespace detail
} // namespace Bitset2




#endif // BITSET2_ARRAY_OPS_CB_HPP

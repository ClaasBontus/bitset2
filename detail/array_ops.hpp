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
#include "reverse_bits.hpp"


namespace Bitset2
{
namespace detail
{

  template<size_t N>
  struct array_ops
  {
    using this_t=                      array_ops<N>;
    using b_chars=                     bit_chars<N>;
    using ULLONG=                      h_types::ULLONG;
    enum : size_t {   ullong_bits=     b_chars::ullong_bits
                    , n_ullong=        b_chars::n_ullong
                    , n_array=         b_chars::n_array
                    , mod_val=         b_chars::mod_val
                    , n_m_mod=         mod_val == 0 ? 0 : ullong_bits - mod_val
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

    void
    shift_left_assgn( array_t &arr ) const noexcept
    {
      for( size_t c= n_ullong; c > 0; )
      {
        --c;
        if( c >= m_shft_div )
        {
          auto const c2= c - m_shft_div;
          ULLONG const v1= arr[c2] << m_shft_mod;
          ULLONG const v2=   ( c2 == 0 ) ? 0ull
                           : ull_right_shift( arr[c2-1] & m_shft_left_pattern,
                                              m_shft_leftright_shift );
          arr[c]= v1 | v2;
        }
        else arr[c]= 0ull;
      } // for c
      arr[n_array-1] &= hgh_bit_pattern;
    } // shift_left_assgn

    void
    shift_right_assgn( array_t &arr ) const noexcept
    {
      for( size_t c= 0; c < n_ullong; ++c )
      {
        auto const c2= c + m_shft_div;
        if( c2 < n_ullong )
        {
          ULLONG const v1= arr[c2] >> m_shft_mod;
          ULLONG const v2=   ( c2 + 1 >= n_ullong ) ? 0ull
                           : ull_left_shift( arr[c2+1] & m_shft_right_pattern,
                                             m_shft_leftright_shift );
          arr[c]= v1 | v2;
        }
        else arr[c]= 0ull;
      } // for c
      arr[n_array-1] &= hgh_bit_pattern;
    } // shift_right_assgn


    constexpr
    array_t
    rotate_left( array_t const &arr ) const noexcept
    {
      return
        ( n_array == 1 )
          ? array_t{{ (   ( arr[0] << m_n_shift_mod )
                        | ( ull_right_shift( arr[0], N - m_n_shift_mod ) )
                      ) & hgh_bit_pattern  }}
          : rotate_left_impl( arr,
                              array_ops<N>(     m_n_shift_mod ),
                              array_ops<N>( N - m_n_shift_mod ),
                              std::make_index_sequence<n_array>() );
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

    /// Reverse bits
    constexpr
    array_t
    reverse( array_t const &arr ) const noexcept
    {
      return   n_array == 1
             ? array_t{{ reverse_bits<ULLONG>()( arr[0] ) >> n_m_mod }}
             : reverse_impl( arr, std::make_index_sequence<n_array>() );
    } // reverse


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
             : ( arr[idx-m_shft_div] << m_shft_mod )
               | h_shift_left_rem( idx - m_shft_div, arr );
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
             : (   ( arr[idx+m_shft_div] >> m_shft_mod )
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
    rotate_left_impl( array_t const &arr,
                      this_t  lft,
                      this_t  rgt,
                      std::index_sequence<S...> ) const noexcept
    {
      return
        {{ ( S > lft.m_shft_div ? lft.h_shift_left( S, arr )
             : S ==  lft.m_shft_div
               ?   lft.h_shift_left(  lft.m_shft_div, arr )
                 | rgt.h_shift_right( lft.m_shft_div, arr )
               : rgt.h_shift_right( S, arr ) )... }};
    } // rotate_left_impl


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


    template<size_t ... S>
    constexpr
    array_t
    reverse_impl( array_t const &arr, std::index_sequence<S...> ) const noexcept
    { return {{ h_reverse( S, arr )... }}; }

    constexpr
    ULLONG
    h_reverse( size_t idx, array_t const &arr ) const noexcept
    {
      return   idx + 1 == n_ullong
             ? reverse_bits<ULLONG>()( arr[0] ) >> n_m_mod
             : reverse_bits<ULLONG>()( h2_reverse( idx, arr ) );
    }

    constexpr
    ULLONG
    h2_reverse( size_t idx, array_t const &arr ) const noexcept
    {
      return   mod_val == 0 ? arr[n_ullong-idx-1]
             :   ( arr[n_ullong-idx-1] << n_m_mod )
               | ( arr[n_ullong-idx-2] >> mod_val );
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

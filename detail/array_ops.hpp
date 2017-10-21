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

  template<size_t N,class T>
  struct array_ops
  {
    using base_t=                      T;
    using this_t=                      array_ops<N,T>;
    using b_chars=                     bit_chars<N,T>;
    enum : size_t {   base_t_n_bits=   b_chars::base_t_n_bits
                    , n_words=         b_chars::n_words
                    , n_array=         b_chars::n_array
                    , mod_val=         b_chars::mod_val
                    , n_m_mod=         mod_val == 0 ? 0 : base_t_n_bits-mod_val
                  };
    enum : base_t
    {   hgh_bit_pattern= b_chars::hgh_bit_pattern
    ,   all_one=         b_chars::all_one };
    using array_t=            typename h_types<T>::template array_t<n_array>;
    using zero_array_t=       typename h_types<T>::template array_t<0>;

    constexpr
    array_ops( size_t n_shift ) noexcept
    : m_n_shift_mod( n_shift % N )
    , m_shft_div( n_shift / base_t_n_bits )
    , m_shft_mod( n_shift % base_t_n_bits )
    , m_shft_leftright_shift( base_t_n_bits-(n_shift % base_t_n_bits) )
    , m_shft_left_pattern( ce_left_shift( T(~T(0)),
                                          base_t_n_bits
                                             - (n_shift % base_t_n_bits) ))
    , m_shft_right_pattern( ce_right_shift( T(~T(0)),
                                            base_t_n_bits
                                             - (n_shift % base_t_n_bits) ))
    {}

    constexpr
    array_t
    shift_left( array_t const &arr ) const noexcept
    { return shift_left_impl( arr, std::make_index_sequence<n_array>() ); }

    constexpr
    array_t
    shift_right( array_t const &arr ) const noexcept
    { return shift_right_impl( arr, std::make_index_sequence<n_array>() ); }

    /// Used for <<= operator. Separate implementation for better performance.
    constexpr
    void
    shift_left_assgn( array_t &arr ) const noexcept
    {
      for( size_t c= n_words; c > 0; )
      {
        --c;
        if( c >= m_shft_div )
        {
          auto const c2= c - m_shft_div;
          base_t const v1= arr[c2] << m_shft_mod;
          base_t const v2=
            c2 == 0 ? base_t(0)
                    : ce_right_shift( base_t(arr[c2-1] & m_shft_left_pattern),
                                      m_shft_leftright_shift );
          arr[c]= base_t( v1 | v2 );
        }
        else arr[c]= base_t(0);
      } // for c
      arr[n_array-1] &= hgh_bit_pattern;
    } // shift_left_assgn

    /// Used for >>= operator. Separate implementation for better performance.
    constexpr
    void
    shift_right_assgn( array_t &arr ) const noexcept
    {
      for( size_t c= 0; c < n_words; ++c )
      {
        auto const c2= c + m_shft_div;
        if( c2 < n_words )
        {
          base_t const v1= arr[c2] >> m_shft_mod;
          base_t const v2=
            ( c2 + 1 >= n_words ) ? base_t(0)
                  : ce_left_shift( base_t( arr[c2+1] & m_shft_right_pattern ),
                                           m_shft_leftright_shift );
          arr[c]= v1 | v2;
        }
        else arr[c]= base_t(0);
      } // for c
      arr[n_array-1] &= hgh_bit_pattern;
    } // shift_right_assgn


    constexpr
    array_t
    rotate_left( array_t const &arr ) const noexcept
    {
      return
        ( n_array == 1 )
          ? array_t{{ base_t((   base_t( arr[0] << m_n_shift_mod )
                               | ( ce_right_shift( arr[0], N - m_n_shift_mod ) )
                             ) & hgh_bit_pattern ) }}
          : rotate_left_impl( arr,
                              array_ops<N,T>(     m_n_shift_mod ),
                              array_ops<N,T>( N - m_n_shift_mod ),
                              std::make_index_sequence<n_array>() );
    } // rotate_left

    constexpr
    array_t
    flip( array_t const &arr ) const noexcept
    { return flip_impl( arr, std::make_index_sequence<n_array>() ); }

    constexpr
    bool
    all( array_t const &arr ) const noexcept
    { return ( N > 0 ) && all_impl( n_words - 1, arr ); }


    /// Used for ++ operator.
    constexpr
    void
    increment( array_t &arr ) const noexcept
    {
      if( N == 0 ) return;

      size_t  c= 0;
      for( ; c + 1 < n_words; ++c )
      {
        if( ( ++arr[c] ) != base_t(0) ) return;
      }
      ++arr[c];
      arr[c] &= hgh_bit_pattern;
    } // increment


    /// Used for -- operator.
    constexpr
    void
    decrement( array_t &arr ) const noexcept
    {
      if( N == 0 ) return;

      size_t  c= 0;
      for( ; c + 1 < n_words; ++c )
      {
        if( ( arr[c]-- ) != base_t(0) ) return;
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
             ? array_t{{ base_t( reverse_bits<base_t>()(arr[0]) >> n_m_mod ) }}
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
      return ( idx + 1 ==  n_words ) ? ( arr[idx] == hgh_bit_pattern )
                                     : ( arr[idx] == all_one );
    }


    template<size_t ... S>
    constexpr
    array_t
    shift_left_impl( array_t const &arr,
                     std::index_sequence<S...> ) const noexcept
    { return {{ h_shift_left( S, arr )... }};  }

    constexpr
    base_t
    h_shift_left( size_t idx, array_t const &arr ) const noexcept
    {
      return   h_shift_left1( idx, arr )
             & ( ( idx+1 == n_words ) ?  hgh_bit_pattern : all_one );
    }

    constexpr
    base_t
    h_shift_left1( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx >= n_words || idx < m_shft_div ) ? base_t(0)
             : base_t( ( arr[idx-m_shft_div] << m_shft_mod )
                       | h_shift_left_rem( idx - m_shft_div, arr ) );
    }

    constexpr
    base_t
    h_shift_left_rem( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx == 0 ) ? base_t(0)
             : ce_right_shift( base_t( arr[idx-1] & m_shft_left_pattern ),
                                m_shft_leftright_shift );
    }


    template<size_t ... S>
    constexpr
    array_t
    shift_right_impl( array_t const &arr,
                      std::index_sequence<S...> ) const noexcept
    { return {{ h_shift_right( S, arr )... }};  }

    constexpr
    base_t
    h_shift_right( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx + m_shft_div >= n_words ) ? base_t(0)
             : base_t(   ( arr[idx+m_shft_div] >> m_shft_mod )
                       | h_shift_right_rem( idx + m_shft_div, arr ) );
    }

    constexpr
    base_t
    h_shift_right_rem( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx + 1 >= n_words ) ? base_t(0)
             : ce_left_shift( base_t( arr[idx+1] & m_shft_right_pattern ),
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
               ? base_t( lft.h_shift_left(  lft.m_shft_div, arr )
                         | rgt.h_shift_right( lft.m_shft_div, arr ) )
               : rgt.h_shift_right( S, arr ) )... }};
    } // rotate_left_impl


    template<size_t ... S>
    constexpr
    array_t
    flip_impl( array_t const &arr, std::index_sequence<S...> ) const noexcept
    { return {{ h_flip( S, arr )... }}; }

    constexpr
    base_t
    h_flip( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx >= n_words ) ? base_t(0)
             :   ( ~arr[idx] )
               & ( (idx+1 == n_words) ? hgh_bit_pattern : all_one );
    }


    template<size_t ... S>
    constexpr
    array_t
    reverse_impl( array_t const &arr, std::index_sequence<S...> ) const noexcept
    { return {{ h_reverse( S, arr )... }}; }

    constexpr
    base_t
    h_reverse( size_t idx, array_t const &arr ) const noexcept
    {
      return   idx + 1 == n_words
             ? base_t( reverse_bits<base_t>()( arr[0] ) >> n_m_mod )
             : reverse_bits<base_t>()( h2_reverse( idx, arr ) );
    }

    constexpr
    base_t
    h2_reverse( size_t idx, array_t const &arr ) const noexcept
    {
      return   mod_val == 0 ? arr[n_words-idx-1]
             : base_t(   ( arr[n_words-idx-1] << n_m_mod )
                       | ( arr[n_words-idx-2] >> mod_val ) );
    }


    size_t const     m_n_shift_mod;
    size_t const     m_shft_div;
    size_t const     m_shft_mod;
    size_t const     m_shft_leftright_shift;
    base_t const     m_shft_left_pattern;
    base_t const     m_shft_right_pattern;

  }; // struct array_ops

} // namespace detail
} // namespace Bitset2




#endif // BITSET2_ARRAY_OPS_CB_HPP

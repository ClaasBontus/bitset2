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

#ifndef BITSET2_ARRAY_ACCESS_CB_HPP
#define BITSET2_ARRAY_ACCESS_CB_HPP

#include "bit_chars.hpp"


namespace Bitset2
{
namespace detail
{

  /// \brief array_access is used for getting bunches of 4 bits, which is
  /// needed when creating hex-strings
  template<size_t N,class T>
  struct array_access
  {
    using base_t=                      T;
    using b_chars=                     bit_chars<N,T>;
    enum : size_t {   base_t_n_bits=   b_chars::base_t_n_bits
                    , n_array=         b_chars::n_array
                  };
    enum : size_t {   div_four=        N / 4
                    , mod_four=        N % 4
                  };
    using array_t=       typename h_types<T>::template array_t<n_array>;

    constexpr
    base_t
    get_four_bits( array_t const &arr, size_t offset ) const noexcept
    {
      return
        get_four_bits_impl( arr,
                            offset / base_t_n_bits, offset % base_t_n_bits );
    }

    constexpr
    base_t
    get_four_bits_impl( array_t const &arr,
                        size_t idx, size_t bit_idx ) const noexcept
    {
      return ( bit_idx >= 3 )
             ? h1_get_four_bits( arr[idx], bit_idx )
             : h2_get_four_bits( arr, idx, bit_idx );
    }

    constexpr
    base_t
    h1_get_four_bits( base_t v, size_t bit_idx ) const noexcept
    { return ( v >> ( bit_idx - 3 ) ) & base_t(0xF); }

    constexpr
    base_t
    h2_get_four_bits( array_t const &arr,
                      size_t idx, size_t bit_idx ) const noexcept
    {
      return
        (( arr[idx] & ( ( base_t(1) << (bit_idx+1) ) - 1 ) ) << ( 3 - bit_idx ))
        + ( ( idx == 0 ) ? base_t(0)
                         : ( arr[idx-1] >> (base_t_n_bits - (3 - bit_idx)) ) );
    }
  }; // struct array_access


} // namespace detail
} // namespace Bitset2




#endif // BITSET2_ARRAY_ACCESS_CB_HPP

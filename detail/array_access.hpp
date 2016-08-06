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
  template<size_t N>
  struct array_access
  {
    using b_chars=                     bit_chars<N>;
    using ULLONG=                      h_types::ULLONG;
    enum : size_t {   ullong_bits=     b_chars::ullong_bits
                    , n_array=         b_chars::n_array
                  };
    enum : size_t {   div_four=        N / 4
                    , mod_four=        N % 4
                  };
    using array_t=                     h_types::array_t<n_array>;

    constexpr
    ULLONG
    get_four_bits( array_t const &arr, size_t offset ) const noexcept
    {
      return get_four_bits_impl( arr,
                                 offset / ullong_bits, offset % ullong_bits );
    }

    constexpr
    ULLONG
    get_four_bits_impl( array_t const &arr,
                        size_t idx, size_t bit_idx ) const noexcept
    {
      return ( bit_idx >= 3 )
             ? h1_get_four_bits( arr[idx], bit_idx )
             : h2_get_four_bits( arr, idx, bit_idx );
    }

    constexpr
    ULLONG
    h1_get_four_bits( ULLONG v, size_t bit_idx ) const noexcept
    { return ( v >> ( bit_idx - 3 ) ) & 0xFull; }

    constexpr
    ULLONG
    h2_get_four_bits( array_t const &arr,
                      size_t idx, size_t bit_idx ) const noexcept
    {
      return
        ( ( arr[idx] & ( ( 1ull << (bit_idx+1) ) - 1 ) ) << ( 3 - bit_idx ) )
        + ( ( idx == 0 ) ? 0ull
            : ( arr[idx-1] >> ( ullong_bits - ( 3 - bit_idx ) ) ) );
    }
  }; // struct array_access


} // namespace detail
} // namespace Bitset2




#endif // BITSET2_ARRAY_ACCESS_CB_HPP

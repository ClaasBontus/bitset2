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

#ifndef BITSET2_BIT_CHARS_CB_HPP
#define BITSET2_BIT_CHARS_CB_HPP

#include "h_types.hpp"


namespace Bitset2
{
namespace detail
{

  template<size_t N>
  struct bit_chars
  {
      using ULONG=  h_types::ULONG;
      using ULLONG= h_types::ULLONG;
      //
      enum : size_t
      { n_bits=       N
      , ulong_bits=   h_types::ulong_bits                ///< #bits in ULONG
      , ullong_bits=  h_types::ullong_bits               ///< #bits in ULLONG
      , ulong_max=    ULONG_MAX
      , div_val=      n_bits / ullong_bits
      , mod_val=      n_bits % ullong_bits
      , n_ullong=     mod_val ? div_val + 1 : div_val    ///< #ULLONG required
      , n_array=      ( n_ullong == 0 ) ? 1 : n_ullong   ///< #ULLONG used
      };
      enum : ULLONG
      { low_bit_pattern=                           ///< Mask for idx==0
              n_bits == 0 ? 0
              : ( div_val > 0 || mod_val == 0 )
                ? (~(0ull))
                : ull_right_shift( ~(0ull), ullong_bits - mod_val )
      , hgh_bit_pattern=                           ///< Mask for idx+1==n_ullong
              n_bits == 0 ? 0
              : mod_val == 0
                ? (~(0ull))
                : ull_right_shift( ~(0ull), ullong_bits - mod_val )
      };
  }; // struct bit_chars

} // namespace detail
} // namespace Bitset2




#endif // BITSET2_BIT_CHARS_CB_HPP

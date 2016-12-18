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

  template<size_t N,class T>
  struct bit_chars
  {
      using h_t=      h_types<T>;
      using ULONG_t=  typename h_t::ULONG_t;
      using ULLONG_t= typename h_t::ULLONG_t;
      using base_t=   T;
      //
      enum : size_t
      { n_bits=         N
      , ulong_n_bits=   h_t::ulong_n_bits                ///< #bits in ULONG_t
      , ullong_n_bits=  h_t::ullong_n_bits               ///< #bits in ULLONG_t
      , base_t_n_bits=  h_t::base_t_n_bits               ///< #bits in T
      , div_val=        n_bits / base_t_n_bits
      , mod_val=        n_bits % base_t_n_bits
      , n_words=        mod_val ? div_val + 1 : div_val  ///< #words required
      , n_array=        ( n_words == 0 ) ? 1 : n_words   ///< #words used
      };
      enum : ULONG_t
      { ulong_max=      ULONG_MAX };
      enum : base_t
      { all_one= base_t(~base_t(0))
      , low_bit_pattern=                         ///< Mask for idx==0
              n_bits == 0 ? base_t(0)
              : ( div_val > 0 || mod_val == 0 )
                ? all_one
                : ce_right_shift( all_one, base_t_n_bits - mod_val )
      , hgh_bit_pattern=                        ///< Mask for idx+1==n_words
              n_bits == 0 ? base_t(0)
              : mod_val == 0
                ? all_one
                : ce_right_shift( all_one, base_t_n_bits - mod_val )
      };
  }; // struct bit_chars

} // namespace detail
} // namespace Bitset2




#endif // BITSET2_BIT_CHARS_CB_HPP

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

#ifndef BITSET2_COUNT_BITS_CB_HPP
#define BITSET2_COUNT_BITS_CB_HPP


#include "h_types.hpp"


namespace Bitset2
{
namespace detail
{

  /// Returns the number of bits set in val
  template<class T>
  constexpr
  inline
  size_t
  count_bits( T val, size_t count= 0 ) noexcept
  {
    return
      ( val == T(0) )
               ? count
               : count_bits( T(val & T( val - T(1) )), // clears lowest set bit
                             count + 1 );
  }

} // namespace detail
} // namespace Bitset2




#endif // BITSET2_COUNT_BITS_CB_HPP

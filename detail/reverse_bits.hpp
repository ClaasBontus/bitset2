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

#ifndef BITSET2_REVERSE_BITS_CB_HPP
#define BITSET2_REVERSE_BITS_CB_HPP

#include <limits>
#include <climits>
#include <cstddef>



namespace Bitset2
{
namespace detail
{


/// https://graphics.stanford.edu/~seander/bithacks.html#ReverseParallel
template<class T>
struct reverse_bits
{
  enum : size_t { n_bits=   sizeof(T) * CHAR_BIT
                , n_bits_h= n_bits >> 1 };

  constexpr
  reverse_bits() noexcept
  {
    static_assert( ( n_bits & ( n_bits - 1 ) ) == 0,
                   "Number of bits in data type is not a power of 2" );
  }

  /// \brief Reverses bits in val.
  constexpr
  T
  operator()( T val ) const noexcept  { return rvrs( val ); }

private:
  constexpr
  T
  rvrs( T       val,
        T       mask= T( ~T(0) ) >> n_bits_h,
        size_t  s=    n_bits_h ) const noexcept
  {
    return   s == 0 ? val
           : rvrs( ( (val >> s) & mask ) | ( (val << s) & ~mask ),
                   mask ^ ( mask << ( s >> 1 ) ),
                   s >> 1 );
  }
}; // struct reverse_bits


} // namespace detail
} // namespace Bitset2


#endif // BITSET2_REVERSE_BITS_CB_HPP

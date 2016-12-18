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


#ifndef BITSET2_INDEX_LSB_SET_CB_HPP
#define BITSET2_INDEX_LSB_SET_CB_HPP


#include <limits>
#include <climits>
#include <cstddef>



namespace Bitset2
{
namespace detail
{


/// https://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightBinSearch
template<class T>
struct index_lsb_set
{
  enum : size_t { npos=   std::numeric_limits<size_t>::max()
                , n_bits= sizeof(T) * CHAR_BIT };

  constexpr
  index_lsb_set() noexcept
  {
    static_assert( ( n_bits & ( n_bits - 1 ) ) == 0,
                   "Number of bits in data type is not a power of 2" );
  }

  /// \brief Returns index of first (least significant) bit set in val.
  /// Returns npos if all bits are zero.
  constexpr
  size_t
  operator()( T val ) const noexcept
  {
    return   ( T(0) == val ) ? npos
           : find_idx( val, T( T(~T(0)) >> (n_bits >> 1) ), n_bits >> 1, 1 );
  }

private:
  constexpr
  size_t
  find_idx( T val, T pttrn, size_t sh_rght, size_t ct ) const noexcept
  {
    return   ( sh_rght == 1 ) ? ( ct - size_t( T(val & T(0x1)) ) )
           : T( val & pttrn ) == T(0)
             ? find_idx( T(val >> sh_rght), T( pttrn >> ( sh_rght >> 1 )),
                         sh_rght >> 1, ct + sh_rght )
             : find_idx( val, T(pttrn >> ( sh_rght >> 1 )), sh_rght >> 1, ct );
  }
}; // struct index_lsb_set



} // namespace detail
} // namespace Bitset2


#endif // BITSET2_INDEX_LSB_SET_CB_HPP

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

#ifndef BITSET2_ARRAY_ADD_CB_HPP
#define BITSET2_ARRAY_ADD_CB_HPP


#include "bit_chars.hpp"
#include "array_funcs.hpp"
#include "array_ops.hpp"


namespace Bitset2
{
namespace detail
{

  /// Allows to add two std::array's in a constexpr
  template<size_t N,class T>
  struct array_add
  {
    using base_t=                      T;
    using b_chars=                     bit_chars<N,T>;
    enum : base_t
    {   hgh_bit_pattern= b_chars::hgh_bit_pattern
    ,   all_one=         b_chars::all_one
    };
    enum : size_t {   n_array=         b_chars::n_array   };
    using array_t=            typename h_types<T>::template array_t<n_array>;

    constexpr
    array_t
    add( array_t arr1, array_t const &arr2 ) const noexcept
    {
      add_assgn( arr1, arr2 );
      return arr1;
    }

    constexpr
    void
    add_assgn( array_t &arr1, array_t const &arr2,
               base_t  carry= base_t(0) ) const noexcept
    {
      for( size_t  c= 0; c < n_array; ++c )
      {
        auto const sm1= base_t( arr2[c] + carry );
        auto const sm=  base_t( arr1[c] + sm1 );
        carry=  sm < arr1[c] || sm1 < arr2[c];
        arr1[c]= sm;
      }
      arr1[n_array-1] &= hgh_bit_pattern;
    } // add_assgn

    constexpr
    array_t
    midpoint( array_t const &arr1, array_t const &arr2,
              bool round_down ) const noexcept
    {
      // https://devblogs.microsoft.com/oldnewthing/20220207-00/?p=106223
      T val0_1 = arr1[0];
      T val0_2 = arr2[0];
      T carry = val0_1 & val0_2 & 1;
      if( !round_down && carry == 0 ) carry = val0_1 & 1;
      array_t ret_val = array_ops<N,T>(1).shift_right( arr1 );
      array_t bs2_h   = array_ops<N,T>(1).shift_right( arr2 );
      add_assgn( ret_val, bs2_h, carry );
      return ret_val;
    } // midpoint
  }; // struct array_add

} // namespace detail
} // namespace Bitset2




#endif // BITSET2_ARRAY_ADD_CB_HPP

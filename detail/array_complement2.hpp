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

#ifndef BITSET2_ARRAY_COMPLEMENT2_CB_HPP
#define BITSET2_ARRAY_COMPLEMENT2_CB_HPP


#include "bit_chars.hpp"
#include "array_funcs.hpp"


namespace Bitset2
{
namespace detail
{

  /// Computes the two's complement of the number encoded in the array
  template<size_t N,class T>
  struct array_complement2
  {
    using base_t=                      T;
    using b_chars=                     bit_chars<N,T>;
    enum : base_t {   hgh_bit_pattern= b_chars::hgh_bit_pattern };
    enum : size_t {   n_array=         b_chars::n_array   };
    using array_t=            typename h_types<T>::template array_t<n_array>;


    constexpr
    array_t
    comp2( array_t arr ) const noexcept
    {
      comp2_assgn( arr );
      return arr;
    }

    constexpr
    void
    comp2_assgn( array_t &arr ) const noexcept
    {
      base_t  carry= base_t(1);
      for( size_t  c= 0; c < n_array; ++c )
      {
        auto const sm= base_t( base_t(~arr[c]) + carry );
        carry=  base_t( (carry == base_t(1)) && ( sm == 0 ) );
        arr[c]= sm;
      } // for c
      arr[n_array-1] &= hgh_bit_pattern;
    } // comp2_assgn

  }; // struct array_complement2


} // namespace detail
} // namespace Bitset2



#endif // BITSET2_ARRAY_COMPLEMENT2_CB_HPP

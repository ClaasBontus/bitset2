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

  template<size_t N,size_t it_n,class T>
  struct array_complement2_base
  {
    using base_t=                      T;
    using b_chars=                     bit_chars<N,T>;
    enum : base_t {   hgh_bit_pattern= b_chars::hgh_bit_pattern };
    enum : size_t {   n_array=         b_chars::n_array   };
    using array_t=            typename h_types<T>::template array_t<n_array>;

    template<size_t n>
    using arr_t= typename h_types<T>::template array_t<n>;

    constexpr
    arr_t<it_n+1>
    comp2_h1( arr_t<it_n> const &so_far, base_t sum ) const noexcept
    {
      return
        array_funcs<it_n,T>().append( so_far
                                    , ( it_n + 1 < n_array )
                                      ? sum
                                      : base_t(sum & hgh_bit_pattern) );
    }

    constexpr
    arr_t<it_n+1>
    comp2_h2( array_t     const &arr,
              arr_t<it_n> const &so_far,
              base_t            carry ) const noexcept
    { return comp2_h1( so_far, base_t( base_t(~arr[it_n]) + carry ) ); }

    constexpr
    arr_t<it_n+1>
    comp2_h3( array_t     const &arr,
              arr_t<it_n> const &so_far ) const noexcept
    {
      return comp2_h2( arr, so_far,
                       it_n == 0
                       ? base_t(1)
                       : base_t(so_far[it_n-1] == 0 && arr[it_n-1] == 0) );
    }

  }; // struct array_complement2_base


  template<size_t N,size_t it_n,class T>
  struct array_complement2_h : array_complement2_base<N,it_n,T>
  {
    using array_t= typename array_complement2_base<N,it_n,T>::array_t;

    template<size_t n>
    using arr_t= typename array_complement2_base<N,it_n,T>::template arr_t<n>;

    constexpr
    arr_t<it_n+1>
    comp2_h4( array_t const &arr ) const noexcept
    {
      return
        this->comp2_h3( arr,
                        array_complement2_h<N,it_n-1,T>().comp2_h4( arr ) );
    }

  }; // struct array_complement2_h


  template<size_t N,class T>
  struct array_complement2_h<N,0,T> : array_complement2_base<N,0,T>
  {
    using array_t= typename array_complement2_base<N,0,T>::array_t;

    template<size_t n>
    using arr_t= typename array_complement2_base<N,0,T>::template arr_t<n>;

    constexpr
    arr_t<1>
    comp2_h4( array_t const &arr ) const noexcept
    { return this->comp2_h3( arr, arr_t<0>{} ); }

  }; // struct array_complement2_h<N,0,T>



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
    comp2( array_t const &arr ) const noexcept
    {
      return
        ( N == 0 ) ? array_t{}
                   : array_complement2_h<N,n_array-1,T>().comp2_h4( arr );
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

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


namespace Bitset2
{
namespace detail
{

  template<size_t N,size_t it_n,class T>
  struct array_add_base
  {
    using base_t=                      T;
    using b_chars=                     bit_chars<N,T>;
    enum : size_t {   n_array=         b_chars::n_array };
    enum : base_t
    {   hgh_bit_pattern= b_chars::hgh_bit_pattern
    ,   all_one=         b_chars::all_one
    };
    using array_t=            typename h_types<T>::template array_t<n_array>;
    using zero_array_t=       typename h_types<T>::template array_t<0>;

    /// Used to submit the curent result of the addition and the carry over
    template<size_t n>
    using array_pair_t= std::pair<base_t,
                                  typename h_types<T>::template array_t<n> >;


    constexpr
    array_pair_t<it_n+1>
    add_h1( base_t a, base_t b, base_t sm1, base_t sum,
            typename
            h_types<T>::template array_t<it_n> const &so_far ) const noexcept
    {
      return
        std::make_pair( ( sum < a || sm1 < b ) ? base_t(1) : base_t(0)
                      , array_funcs<it_n,T>()
                             .append( so_far
                                    , ( it_n + 1 < n_array )
                                            ? sum
                                            : base_t(sum & hgh_bit_pattern) ) );
    }

    constexpr
    array_pair_t<it_n+1>
    add_h2( base_t a, base_t b, array_pair_t<it_n> const &a_p ) const noexcept
    { return add_h1( a, b, b + a_p.first, a + b + a_p.first, a_p.second ); }
  }; //struct array_add_base


  /// \brief This struct is introduced for beeing able to partially
  /// specialize function add_h3.
  template<size_t N,size_t it_n,class T>
  struct array_add_h : public array_add_base<N,it_n,T>
  {
    using array_t= typename array_add_base<N,it_n,T>::array_t;

    template<size_t n>
    using array_pair_t=
          typename array_add_base<N,it_n,T>::template array_pair_t<n>;

    constexpr
    array_pair_t<it_n+1>
    add_h3( array_t const &arr1, array_t const &arr2 ) const noexcept
    {
      return
        this->add_h2( arr1[it_n], arr2[it_n]
                    , array_add_h<N,it_n-1,T>().add_h3( arr1, arr2 ) );
    }
  }; // struct array_add_h


  template<size_t N,class T>
  struct array_add_h<N,0,T> : public array_add_base<N,0,T>
  {
    using array_t=      typename array_add_base<N,0,T>::array_t;
    using zero_array_t= typename array_add_base<N,0,T>::zero_array_t;

    template<size_t n>
    using array_pair_t=typename array_add_base<N,0,T>::template array_pair_t<n>;

    constexpr
    array_pair_t<1>
    add_h3( array_t const &arr1, array_t const &arr2 ) const noexcept
    {
      return
          this->add_h2( arr1[0], arr2[0]
                      , std::make_pair( T(0), zero_array_t{} ) );
    }
  }; // struct array_add_h


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
    add( array_t const &arr1, array_t const &arr2 ) const noexcept
    {
      return
        ( N == 0 ) ? array_t{}
                   : array_add_h<N,n_array-1,T>().add_h3( arr1, arr2 ).second;
    }

    constexpr
    void
    add_assgn( array_t &arr1, array_t const &arr2 ) const noexcept
    {
      base_t  carry= base_t(0);
      for( size_t  c= 0; c < n_array; ++c )
      {
        auto const sm1= base_t( arr2[c] + carry );
        auto const sm=  base_t( arr1[c] + sm1 );
        carry=  sm < arr1[c] || sm1 < arr2[c];
        arr1[c]= sm;
      }
      arr1[n_array-1] &= hgh_bit_pattern;
    } // add_assgn
  }; // struct array_add

} // namespace detail
} // namespace Bitset2




#endif // BITSET2_ARRAY_ADD_CB_HPP

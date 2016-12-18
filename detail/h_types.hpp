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

#ifndef BITSET2_H_TYPES_CB_HPP
#define BITSET2_H_TYPES_CB_HPP

#include <array>
#include <utility>
#include <climits>
#include <type_traits>


namespace Bitset2
{
namespace detail
{

    template<class T,class Enabled=void>      struct h_types;

    template<class T>
    struct h_types<T,
                   typename std::enable_if<   std::is_integral<T>::value
                                           && std::is_unsigned<T>::value>::type>
    {
      using ULONG_t=  unsigned long;
      using ULLONG_t= unsigned long long;
      using base_t=   T;

      template<size_t n_array>
      using array_t= std::array<base_t,n_array>;

      enum : size_t
      { ulong_n_bits=   sizeof(ULONG_t)  * CHAR_BIT  ///< #bits in ULONG_t
      , ullong_n_bits=  sizeof(ULLONG_t) * CHAR_BIT  ///< #bits in ULLONG_t
      , base_t_n_bits=  sizeof(base_t)   * CHAR_BIT  ///< #bits in base_t
      , npos=          ~size_t(0)
      };
    }; // struct h_types


    template<class T>
    constexpr
    T
    ce_min( T v1, T v2 ) noexcept
    { return ( v1 < v2 ) ? v1 : v2; }



    /// http://stackoverflow.com/q/29136207/3876684
    template<class T>
    constexpr
    T
    ce_left_shift( T v1, size_t n_shift ) noexcept
    {
      return   ( n_shift == 0 ) ? v1
             : ( ( n_shift >= h_types<T>::base_t_n_bits ) ? T(0)
               : T( v1 << n_shift ) );
    }

    template<class T>
    constexpr
    T
    ce_right_shift( T v1, size_t n_shift ) noexcept
    {
      return   ( n_shift == 0 ) ? v1
             : ( ( n_shift >= h_types<T>::base_t_n_bits ) ? T(0)
               : T( v1 >> n_shift ) );
    }



    template<size_t n_array,class T,size_t ... S>
    inline constexpr
    typename h_types<T>::template array_t<n_array>
    gen_empty_array_impl( std::index_sequence<S...> ) noexcept
    {
      return
        typename h_types<T>::template array_t<n_array>{{ ( T(S) & T(0) ) ... }};
    }



    template<size_t n_array,class T>
    inline constexpr
    typename h_types<T>::template array_t<n_array>
    gen_empty_array() noexcept
    {
      return
        gen_empty_array_impl<n_array,T>( std::make_index_sequence<n_array>() );
    } // gen_empty_array

} // namespace detail
} // namespace Bitset2




#endif // BITSET2_H_TYPES_CB_HPP

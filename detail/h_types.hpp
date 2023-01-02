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
#include <cstddef>


namespace Bitset2
{
namespace detail
{
    template<class T>
    struct is_unsgnd_int
    {
        static
        constexpr bool value = (  std::is_integral<T>::value
                               && std::is_unsigned<T>::value )
#ifdef __SIZEOF_INT128__
                               || std::is_same<T,unsigned __int128>::value
#endif
        ;
    }; // struct is_unsgnd_int

    
    template<class T,class Enabled=void>      struct h_types;

    template<class T>
    struct h_types<T,
                   typename std::enable_if<is_unsgnd_int<T>::value>::type>
    {
      using ULONG_t=  unsigned long;
      using ULLONG_t= unsigned long long;
      using base_t=   T;
#ifdef __SIZEOF_INT128__
      using LRGST_t=  unsigned __int128;
#else
      using LRGST_t=  ULLONG_t;
#endif

      template<size_t n_array>
      using array_t= std::array<base_t,n_array>;

      enum : size_t
      { ulong_n_bits=   sizeof(ULONG_t)  * CHAR_BIT  ///< #bits in ULONG_t
      , ullong_n_bits=  sizeof(ULLONG_t) * CHAR_BIT  ///< #bits in ULLONG_t
      , lrgst_n_bist=   sizeof(LRGST_t)  * CHAR_BIT  ///< #bits in LRGST_t
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

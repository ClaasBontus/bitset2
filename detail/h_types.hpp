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


namespace Bitset2
{
namespace detail
{

    struct h_types
    {
      using ULONG=  unsigned long;
      using ULLONG= unsigned long long;

      template<size_t n_array>
      using array_t= std::array<ULLONG,n_array>;
      
      enum : size_t
      { ulong_bits=   sizeof(ULONG)  * CHAR_BIT          ///< #bits in ULONG
      , ullong_bits=  sizeof(ULLONG) * CHAR_BIT          ///< #bits in ULLONG
      };
    }; // struct h_types
    
    
    constexpr
    h_types::ULLONG
    ull_min( h_types::ULLONG v1, h_types::ULLONG v2 ) noexcept
    { return ( v1 < v2 ) ? v1 : v2; }
    
    
    
    // http://stackoverflow.com/q/29136207/3876684
    constexpr
    h_types::ULLONG
    ull_left_shift( h_types::ULLONG v1, size_t n_shift ) noexcept
    {
      return   ( n_shift == 0 ) ? v1
             : ( ( n_shift >= h_types::ullong_bits ) ? 0ull
               : ( v1 << n_shift ) );
    }
    
    constexpr
    h_types::ULLONG
    ull_right_shift( h_types::ULLONG v1, size_t n_shift ) noexcept
    {
      return   ( n_shift == 0 ) ? v1
             : ( ( n_shift >= h_types::ullong_bits ) ? 0ull
               : ( v1 >> n_shift ) );
    }
    
    
    
    template<size_t n_array,size_t ... S>
    inline constexpr
    h_types::array_t<n_array>
    gen_empty_array_impl( std::index_sequence<S...> ) noexcept
    {
      using ULLONG= h_types::ULLONG;
      return h_types::array_t<n_array>{{ ( ULLONG(S) & 0ull ) ... }};
    }
    
    
    
    template<size_t n_array>
    inline constexpr
    h_types::array_t<n_array>
    gen_empty_array() noexcept
    {
      return 
        gen_empty_array_impl<n_array>( std::make_index_sequence<n_array>() );
    } // gen_empty_array

} // namespace detail
} // namespace Bitset2




#endif // BITSET2_H_TYPES_CB_HPP

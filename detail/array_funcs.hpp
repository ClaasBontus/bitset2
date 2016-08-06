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

#ifndef BITSET2_ARRAY_FUNCS_CB_HPP
#define BITSET2_ARRAY_FUNCS_CB_HPP


#include "h_types.hpp"
#include "count_bits.hpp"
#include <utility>


namespace Bitset2
{
namespace detail
{

  template<size_t n_array>
  struct array_funcs
  {
      using ULLONG=                      h_types::ULLONG;
      using array_t=                     h_types::array_t<n_array>;
      using array_p1_t=                  h_types::array_t<n_array+1>;

      /// Binary operator type
      enum class op_type { or_op, and_op, xor_op };

      constexpr
      array_t
      bitwise_or( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
          return bitwise_op_impl( op_type::or_op, arr1, arr2,
                                  std::make_index_sequence<n_array>() );
      }

      constexpr
      array_t
      bitwise_and( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
        return bitwise_op_impl( op_type::and_op, arr1, arr2,
                                std::make_index_sequence<n_array>() );
      }

      constexpr
      array_t
      bitwise_xor( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
        return bitwise_op_impl( op_type::xor_op, arr1, arr2,
                                std::make_index_sequence<n_array>() );
      }

      constexpr
      bool
      none( array_t const &arr ) const noexcept
      { return none_impl( n_array - 1, arr ); }


      constexpr
      size_t
      count( array_t const &arr ) const noexcept
      {
        return
          sum_impl( count_impl( arr, std::make_index_sequence<n_array>() ) );
      }


      constexpr
      bool
      equal( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
        return
            and_impl(equal_impl( arr1, arr2,
                                 std::make_index_sequence<n_array>()) );
      }


      /// Prepend v1 in front of arr
      constexpr
      array_p1_t
      prepend( ULLONG const v1, array_t const &arr ) const noexcept
      { return prepend_impl( v1, arr, std::make_index_sequence<n_array>()); }


      /// Append v1 to arr
      constexpr
      array_p1_t
      append( array_t const &arr, ULLONG const v1 ) const noexcept
      { return append_impl( arr, v1, std::make_index_sequence<n_array>()); }


      /// Copy each element in arr but apply pttrn to most significant entry
      template<size_t n>
      constexpr
      array_t
      copy_and_map( ULLONG const pttrn, 
                    h_types::array_t<n> const &arr ) const noexcept
      {
        return 
          n_array == 0 ? array_t{}
             : copy_and_map_impl( pttrn,
                                  arr,
                                  gen_empty_array<n_array>(),
                                  n >= n_array,
                                  std::make_index_sequence<ull_min( n_array-1, n )>(),
                                  std::make_index_sequence<n_array-1-ull_min( n_array-1, n )>() );
      }


      //** _impl functions

      template<size_t n,size_t ... S,size_t ... T>
      constexpr
      array_t
      copy_and_map_impl( ULLONG const              pttrn, 
                         h_types::array_t<n> const &arr,
                         array_t const             &zeroes,
                         bool const                take_all,
                         std::index_sequence<S...>,
                         std::index_sequence<T...> ) const noexcept
      { return {{ arr[S]..., zeroes[T]..., 
                 ( take_all ? arr[n_array-1] : 0ull ) & pttrn }};  }



      template<size_t ... S>
      constexpr
      array_p1_t
      append_impl( array_t const &arr, ULLONG const v1,
      std::index_sequence<S...> ) const noexcept
      { return {{ arr[S]..., v1 }}; }


      constexpr
      bool
      none_impl( size_t idx, array_t const &arr ) const noexcept
      {
        return    ( arr[idx] == 0 )
               && ( ( idx == 0 ) ? true : none_impl( idx - 1, arr )  );
      }


      template<size_t ... S>
      constexpr
      array_p1_t
      prepend_impl( ULLONG const v1, array_t const &arr,
                    std::index_sequence<S...> ) const noexcept
      { return {{ v1, arr[S]... }}; }



      template<size_t ... S>
      constexpr
      std::array<bool, n_array>
      equal_impl( array_t const &arr1, array_t const &arr2,
                  std::index_sequence<S...> ) const noexcept
      { return {{ ( arr1[S] == arr2[S] )... }};  }


      template<size_t ... S>
      constexpr
      array_t
      bitwise_op_impl( op_type opt,
                       array_t const &arr1, array_t const &arr2,
                       std::index_sequence<S...> ) const noexcept
      { return {{ h_bitwise_op( S, opt, arr1, arr2 )... }};  }

      constexpr
      ULLONG
      h_bitwise_op( size_t idx, op_type opt,
                     array_t const &arr1, array_t const &arr2 ) const noexcept
      {
        return   (   opt == op_type::or_op )  ? ( arr1[idx] | arr2[idx] )
               : ( ( opt == op_type::and_op ) ? ( arr1[idx] & arr2[idx] )
                                              : ( arr1[idx] ^ arr2[idx] ) );
      }

      /// Count bits in each element of arr
      template<size_t ... S>
      constexpr
      std::array<size_t, n_array>
      count_impl( array_t const &arr, std::index_sequence<S...> ) const noexcept
      { return {{ count_bits( arr[S] )... }};  }


      /// Sum over all elements in vals
      template<class T>
      constexpr
      T
      sum_impl( std::array<T, n_array> const &vals,
                size_t ct= n_array - 1 ) const noexcept
      {
          return vals[ct] + ( ( ct == 0 ) ? 0 : sum_impl( vals, ct - 1 ) );
      }

      /// Returns true if each element of vals is true
      constexpr
      bool
      and_impl( std::array<bool, n_array> const &vals,
                size_t ct= n_array - 1 ) const noexcept
      {
          return vals[ct] && ( ( ct == 0 ) ? true : and_impl( vals, ct - 1 ) );
      }
  }; // struct array_funcs

} // namespace detail

} // namespace Bitset2




#endif // BITSET2_ARRAY_FUNCS_CB_HPP

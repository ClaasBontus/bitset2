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
#include "index_lsb_set.hpp"
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

      enum : size_t { ullong_bits=  h_types::ullong_bits
                    , npos=         h_types::npos };

      /// Binary operator type
      enum class op_type { or_op, and_op, xor_op, sdiff_op };

      constexpr
      array_t
      bitwise_or( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
          return bitwise_op_impl( op_type::or_op, arr1, arr2,
                                  std::make_index_sequence<n_array>() );
      }

      void
      bitwise_or_assgn( array_t &arr1, array_t const &arr2 ) const noexcept
      { return bitwise_op_assgn_impl( op_type::or_op, arr1, arr2 ); }

      constexpr
      array_t
      bitwise_and( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
        return bitwise_op_impl( op_type::and_op, arr1, arr2,
                                std::make_index_sequence<n_array>() );
      }

      void
      bitwise_and_assgn( array_t &arr1, array_t const &arr2 ) const noexcept
      { return bitwise_op_assgn_impl( op_type::and_op, arr1, arr2 ); }

      constexpr
      array_t
      bitwise_xor( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
        return bitwise_op_impl( op_type::xor_op, arr1, arr2,
                                std::make_index_sequence<n_array>() );
      }

      void
      bitwise_xor_assgn( array_t &arr1, array_t const &arr2 ) const noexcept
      { return bitwise_op_assgn_impl( op_type::xor_op, arr1, arr2 ); }

      constexpr
      array_t
      bitwise_setdiff( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
        return bitwise_op_impl( op_type::sdiff_op, arr1, arr2,
                                std::make_index_sequence<n_array>() );
      }

      void
      bitwise_setdiff_assgn( array_t &arr1, array_t const &arr2 ) const noexcept
      { return bitwise_op_assgn_impl( op_type::sdiff_op, arr1, arr2 ); }


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
      { return equal_impl( arr1, arr2, 0 ); }


      constexpr
      bool
      less_than( array_t const &arr1, array_t const &arr2 ) const noexcept
      { return less_than_impl( arr1, arr2, n_array - 1 ); }


      /// \brief Returns true if f returns true for each pair
      /// of elements in arr1 and arr2
      template<class F>
      bool
      zip_fold_and( array_t const &arr1, array_t const &arr2,
                    F &f ) const noexcept(noexcept( f( 0ull, 0ull ) ))
      { return zip_fold_and_impl( arr1, arr2, f, 0 ); }


      /// \brief Returns true if f returns true for at least one pair
      /// of elements in arr1 and arr2
      template<class F>
      bool
      zip_fold_or( array_t const &arr1, array_t const &arr2,
                   F &f ) const noexcept(noexcept( f( 0ull, 0ull ) ))
      { return zip_fold_or_impl( arr1, arr2, f, 0 ); }


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
             : copy_and_map_impl(
                 pttrn,
                 arr,
                 gen_empty_array<n_array>(),
                 n >= n_array,
                 std::make_index_sequence<ull_min( n_array-1, n )>(),
                 std::make_index_sequence<n_array-1-ull_min(n_array-1, n)>() );
      } // copy_and_map


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
      {
        return {{ arr[S]..., zeroes[T]...,
                 ( take_all ? arr[n_array-1] : 0ull ) & pttrn }};
      }



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
      append_impl( array_t const &arr, ULLONG const v1,
                   std::index_sequence<S...> ) const noexcept
      { return {{ arr[S]..., v1 }}; }


      template<size_t ... S>
      constexpr
      array_p1_t
      prepend_impl( ULLONG const v1, array_t const &arr,
                    std::index_sequence<S...> ) const noexcept
      { return {{ v1, arr[S]... }}; }


      constexpr
      bool
      equal_impl( array_t const &arr1, array_t const &arr2,
                  size_t ct ) const noexcept
      {
        return   ( ct == n_array ) ? true
               : ( arr1[ct] == arr2[ct] ) && equal_impl( arr1, arr2, ct + 1 );
      }


      constexpr
      bool
      less_than_impl( array_t const &arr1, array_t const &arr2,
                      size_t ct ) const noexcept
      {
        return
          ( arr1[ct] < arr2[ct] )
          || ( arr1[ct] == arr2[ct]
               && ( ct == 0 ? false : less_than_impl( arr1, arr2, ct-1 ) ) );
      }


      template<class F>
      bool
      zip_fold_and_impl( array_t const &arr1, array_t const &arr2,
                         F &f,
                         size_t ct ) const noexcept(noexcept( f( 0ull, 0ull )))
      {
        return   ( ct == n_array ) ? true
               : (    f( arr1[ct], arr2[ct] )
                   && zip_fold_and_impl( arr1, arr2, f, ct + 1 ) );
      }


      template<class F>
      bool
      zip_fold_or_impl( array_t const &arr1, array_t const &arr2,
                        F &f,
                        size_t ct ) const noexcept(noexcept( f( 0ull, 0ull )))
      {
        return   ( ct == n_array ) ? false
               : (    f( arr1[ct], arr2[ct] )
                   || zip_fold_or_impl( arr1, arr2, f, ct + 1 ) );
      }


      void
      bitwise_op_assgn_impl( op_type opt,
                             array_t &arr1, array_t const &arr2 ) const noexcept
      {
        for( size_t c= 0; c < n_array; ++c )
        {
          if(      opt == op_type::or_op  ) arr1[c] |=  arr2[c];
          else if( opt == op_type::and_op ) arr1[c] &=  arr2[c];
          else if( opt == op_type::xor_op ) arr1[c] ^=  arr2[c];
          else                              arr1[c] &= ~arr2[c];
        }
      }


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
                   : ( opt == op_type::xor_op ) ? ( arr1[idx] ^ arr2[idx] )
                                                : ( arr1[idx] & ~arr2[idx] ) );
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
      { return vals[ct] + ( ( ct == 0 ) ? 0 : sum_impl( vals, ct - 1 ) ); }


      constexpr
      size_t
      idx_lsb_set( array_t const &arr, ULLONG v, size_t idx ) const noexcept
      {
        return
          v == 0ull
            ? ( idx + 1 == n_array ? npos
                                   : idx_lsb_set( arr, arr[idx+1], idx + 1 ) )
            : idx * ullong_bits + index_lsb_set<ULLONG>()( v );
      }

  }; // struct array_funcs

} // namespace detail

} // namespace Bitset2




#endif // BITSET2_ARRAY_FUNCS_CB_HPP

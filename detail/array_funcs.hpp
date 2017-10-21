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

  template<size_t n_array,class T>
  struct array_funcs
  {
      using base_t=                 T;
      using array_t=       typename h_types<T>::template array_t<n_array>;
      using array_p1_t=    typename h_types<T>::template array_t<n_array+1>;

      enum : size_t { base_t_n_bits=  h_types<T>::base_t_n_bits
                    , npos=           h_types<T>::npos };

      /// Binary operator type
      enum class op_type { or_op, and_op, xor_op, sdiff_op };

      constexpr
      array_t
      bitwise_or( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
          return bitwise_op_impl( op_type::or_op, arr1, arr2,
                                  std::make_index_sequence<n_array>() );
      }

      /// Used for |= operator. Separate implementation for better performance.
      constexpr
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

      /// Used for &= operator. Separate implementation for better performance.
      constexpr
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

      /// Used for ^= operator. Separate implementation for better performance.
      constexpr
      void
      bitwise_xor_assgn( array_t &arr1, array_t const &arr2 ) const noexcept
      { return bitwise_op_assgn_impl( op_type::xor_op, arr1, arr2 ); }

      /// Computes the set difference, i.e. arr1 & ~arr2
      constexpr
      array_t
      bitwise_setdiff( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
        return bitwise_op_impl( op_type::sdiff_op, arr1, arr2,
                                std::make_index_sequence<n_array>() );
      }

      /// \brief Computes the set difference, i.e. arr1 & ~arr2.
      /// Separate implementation for better performance.
      constexpr
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
      constexpr
      bool
      zip_fold_and( array_t const &arr1, array_t const &arr2,
                    F &f ) const noexcept(noexcept( f( base_t(0), base_t(0) ) ))
      { return zip_fold_and_impl( arr1, arr2, f, 0 ); }


      /// \brief Returns true if f returns true for at least one pair
      /// of elements in arr1 and arr2
      template<class F>
      constexpr
      bool
      zip_fold_or( array_t const &arr1, array_t const &arr2,
                   F &f ) const noexcept(noexcept( f( base_t(0), base_t(0) ) ))
      { return zip_fold_or_impl( arr1, arr2, f, 0 ); }


      /// Prepend v1 in front of arr
      constexpr
      array_p1_t
      prepend( base_t const v1, array_t const &arr ) const noexcept
      { return prepend_impl( v1, arr, std::make_index_sequence<n_array>()); }


      /// Append v1 to arr
      constexpr
      array_p1_t
      append( array_t const &arr, base_t const v1 ) const noexcept
      { return append_impl( arr, v1, std::make_index_sequence<n_array>()); }


      /// Copy each element in arr but apply pttrn to most significant entry
      template<size_t n>
      constexpr
      array_t
      copy_and_map( base_t const                          pttrn,
                    typename
                    h_types<T>::template array_t<n> const &arr ) const noexcept
      {
        return
          n_array == 0 ? array_t{}
             : copy_and_map_impl(
                 pttrn,
                 arr,
                 gen_empty_array<n_array,T>(),
                 n >= n_array,
                 std::make_index_sequence<ce_min( n_array-1, n )>(),
                 std::make_index_sequence<n_array-1-ce_min(n_array-1, n)>() );
      } // copy_and_map


      //** _impl functions

      template<size_t n,size_t ... S1,size_t ... S2>
      constexpr
      array_t
      copy_and_map_impl( base_t const                          pttrn,
                         typename
                         h_types<T>::template array_t<n> const &arr,
                         array_t const                         &zeroes,
                         bool const                            take_all,
                         std::index_sequence<S1...>,
                         std::index_sequence<S2...> ) const noexcept
      {
        return {{ arr[S1]..., zeroes[S2]...,
                  base_t(( take_all ? arr[n_array-1] : base_t(0) ) & pttrn) }};
      }



      constexpr
      bool
      none_impl( size_t idx, array_t const &arr ) const noexcept
      {
        return    ( arr[idx] == base_t(0) )
               && ( ( idx == 0 ) ? true : none_impl( idx - 1, arr )  );
      }


      template<size_t ... S>
      constexpr
      array_p1_t
      append_impl( array_t const &arr, base_t const v1,
                   std::index_sequence<S...> ) const noexcept
      { return {{ arr[S]..., v1 }}; }


      template<size_t ... S>
      constexpr
      array_p1_t
      prepend_impl( base_t const v1, array_t const &arr,
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
      constexpr
      bool
      zip_fold_and_impl( array_t const &arr1, array_t const &arr2,
                         F      &f,
                         size_t ct )
                            const noexcept(noexcept( f(base_t(0), base_t(0))))
      {
        return   ( ct == n_array ) ? true
               : (    f( arr1[ct], arr2[ct] )
                   && zip_fold_and_impl( arr1, arr2, f, ct + 1 ) );
      }


      template<class F>
      constexpr
      bool
      zip_fold_or_impl( array_t const &arr1, array_t const &arr2,
                        F      &f,
                        size_t ct )
                            const noexcept(noexcept( f(base_t(0), base_t(0))))
      {
        return   ( ct == n_array ) ? false
               : (    f( arr1[ct], arr2[ct] )
                   || zip_fold_or_impl( arr1, arr2, f, ct + 1 ) );
      }


      constexpr
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
      } // bitwise_op_assgn_impl


      template<size_t ... S>
      constexpr
      array_t
      bitwise_op_impl( op_type opt,
                       array_t const &arr1, array_t const &arr2,
                       std::index_sequence<S...> ) const noexcept
      { return {{ h_bitwise_op( S, opt, arr1, arr2 )... }};  }

      constexpr
      base_t
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
      template<class T1>
      constexpr
      T1
      sum_impl( std::array<T1, n_array> const &vals,
                size_t ct= n_array - 1 ) const noexcept
      { return vals[ct] + ( ( ct == 0 ) ? T1(0) : sum_impl( vals, ct - 1 ) ); }


      constexpr
      size_t
      idx_lsb_set( array_t const &arr, base_t v, size_t idx ) const noexcept
      {
        return
          v == base_t(0)
            ? ( idx + 1 == n_array ? npos
                                   : idx_lsb_set( arr, arr[idx+1], idx + 1 ) )
            : idx * base_t_n_bits + index_lsb_set<base_t>()( v );
      }

  }; // struct array_funcs

} // namespace detail

} // namespace Bitset2




#endif // BITSET2_ARRAY_FUNCS_CB_HPP

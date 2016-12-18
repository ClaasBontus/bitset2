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


#ifndef BITSET2_ARRAY2U_LONG_T_CB_HPP
#define BITSET2_ARRAY2U_LONG_T_CB_HPP

#include "bit_chars.hpp"


namespace Bitset2
{
namespace detail
{

  /// \brief Takes a std::array 'arr' and returns a variable 'x' of type Tlong.
  /// Bits in 'x' are set if corresponding bits in 'arr' are set.
  template<size_t N, class T, class Tlong>
  struct array2u_long_t
  {
    using  base_t=    T;
    using  b_c=       bit_chars<N,T>;

    enum : size_t
    { n_bits= N
    , base_t_n_bits=  b_c::base_t_n_bits
    , long_t_n_bits=  sizeof( Tlong ) * CHAR_BIT
    , n_array=        b_c::n_array
    , div_val=        long_t_n_bits / base_t_n_bits
    , mod_val=        long_t_n_bits % base_t_n_bits
    , use_vals=       ce_min( div_val + (mod_val!=0), n_array )
    , bit_diff=       mod_val == 0 ? 0 : ( base_t_n_bits - mod_val )
    };

    enum : base_t
    { allset=  base_t( ~base_t(0) )
    , h_pttrn= use_vals < n_array ? allset : ce_right_shift( allset, bit_diff )
    , i_pttrn= base_t( ~h_pttrn )
    };

    using array_t= typename h_types<T>::template array_t<n_array>;

    constexpr
    Tlong
    operator()( array_t const & arr ) const noexcept
    {
      return
        base_t_n_bits >= long_t_n_bits ? Tlong( arr[0] )
                                       : combine( Tlong(0), arr, 0 );
    }

    /// \brief Returns true if arr cannot be converted to Tlong.
    constexpr
    bool
    check_overflow( array_t const & arr ) const noexcept
    { return N <= long_t_n_bits ? false : check_impl( arr, use_vals - 1 ); }



    constexpr
    Tlong
    combine( Tlong v, array_t const & arr, size_t idx ) const noexcept
    {
      return ( idx >= use_vals ) ? v
               : idx + 1 == use_vals
                 ? Tlong( v + take_v( arr, idx, h_pttrn ) )
                 : combine( Tlong( v + take_v( arr, idx ) ), arr, idx + 1 );
    }

    constexpr
    Tlong
    take_v( array_t const & arr, size_t idx, base_t pttrn= allset ) const noexcept
    { return ce_left_shift( Tlong( arr[idx] & pttrn ), idx * base_t_n_bits ); }

    constexpr
    bool
    check_impl( array_t const & arr, size_t idx ) const noexcept
    {
      return idx >= n_array ? false
             : ( ( take_check( arr, idx ) != base_t(0) )
                 || check_impl( arr, idx + 1 ) );
    }

    constexpr
    base_t
    take_check( array_t const & arr, size_t idx ) const noexcept
    { return arr[idx] & ( idx + 1 == use_vals ? i_pttrn : allset ); }

  }; // struct array2u_long_t


} // namespace detail
} // namespace Bitset2







#endif // BITSET2_ARRAY2U_LONG_T_CB_HPP

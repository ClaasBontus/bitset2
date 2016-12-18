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


#ifndef BITSET2_ULLONG2ARRAY_CB_HPP
#define BITSET2_ULLONG2ARRAY_CB_HPP

#include "bit_chars.hpp"


namespace Bitset2
{
namespace detail
{

  /// \brief Takes a variable 'v' of type unsigned long long
  /// and returns a std::array 'a' equivalent to v. 'a' represents
  /// an N bit bitset2 with base_t == T.
  template<size_t N,class T>
  struct ullong2array
  {
    using  base_t=    T;
    using  b_c=       bit_chars<N,T>;
    using  ULLONG_t=  typename b_c::ULLONG_t;

    enum : size_t
    { n_bits= N
    , base_t_n_bits=  b_c::base_t_n_bits
    , ullong_n_bits=  b_c::ullong_n_bits
    , n_array=        b_c::n_array
    , centrl_i=       ce_min( (ullong_n_bits-1) / base_t_n_bits, n_array - 1 )
    , n_empty_vals=   n_array - centrl_i - 1
    };

    enum : base_t
    { hgh_bit_pattern= b_c::hgh_bit_pattern
    , use_pattern=     (n_empty_vals==0) ? hgh_bit_pattern : base_t(~base_t(0))
    };

    using array_t= typename h_types<T>::template array_t<n_array>;

    constexpr
    array_t
    operator()( ULLONG_t v ) const noexcept
    {
      return fill( gen_empty_array<n_array,T>(), v,
                   std::make_index_sequence<n_empty_vals>(),
                   std::make_index_sequence<centrl_i>() );
    }

    template<size_t ... S1,size_t ... S2>
    constexpr
    array_t
    fill( array_t const & empty, ULLONG_t v,
          std::index_sequence<S1...>,
          std::index_sequence<S2...> ) const noexcept
    {
      return {{ base_t(ce_right_shift(v,       S2 * base_t_n_bits))...,
                base_t(ce_right_shift(v, centrl_i * base_t_n_bits)&use_pattern),
                empty[S1]... }};
    }
  }; // struct ullong2array


} // namespace detail
} // namespace Bitset2




#endif // BITSET2_ULLONG2ARRAY_CB_HPP

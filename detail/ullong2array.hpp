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

  
  /// \brief Takes a variable 'v' of type LRGST_t 
  /// (unsigned long long or unsigned __int128)
  /// and returns a std::array 'a' equivalent to v. 'a' represents
  /// an N bit bitset2 with base_t == T.
  template<size_t N,class T>
  struct lrgst2array
  {
    using  base_t=    T;
    using  b_c=       bit_chars<N,T>;
    using  LRGST_t=   typename b_c::LRGST_t;

    enum : size_t
    { n_bits= N
    , base_t_n_bits=  b_c::base_t_n_bits
    , lrgst_n_bist=   b_c::lrgst_n_bist
    , n_array=        b_c::n_array
    , centrl_i=       ce_min( (lrgst_n_bist-1) / base_t_n_bits, n_array - 1 )
    , n_empty_vals=   n_array - centrl_i - 1
    };

    enum : base_t
    { hgh_bit_pattern= b_c::hgh_bit_pattern
    , use_pattern=     (n_empty_vals==0) ? hgh_bit_pattern : base_t(~base_t(0))
    };

    using array_t= typename h_types<T>::template array_t<n_array>;

    constexpr
    array_t
    operator()( LRGST_t v ) const noexcept
    {
      array_t   retval= (detail::gen_empty_array<n_array,T>)();
      size_t    c= 0;
      for( ; c < centrl_i; ++c )
      {
          retval[c]= base_t(v);
          v = ce_right_shift( v, base_t_n_bits );
      }
      retval[c]= base_t( v & use_pattern );
      return retval;
    }
  }; // struct lrgst2array


} // namespace detail
} // namespace Bitset2




#endif // BITSET2_ULLONG2ARRAY_CB_HPP

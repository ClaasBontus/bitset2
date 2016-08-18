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

#ifndef BITSET2_HASH_CB_HPP
#define BITSET2_HASH_CB_HPP

#include "h_types.hpp"

#include <functional>
#include <array>
#include <climits>


namespace Bitset2
{
namespace detail
{


template<size_t n_ullong>
struct hash_impl
{
  using    ULLONG=        h_types::ULLONG;
  using    result_type=   std::size_t;
  using    array_t=       h_types::array_t<n_ullong>;

  enum : size_t
  { size_t_bits=  sizeof(result_type) * CHAR_BIT     ///< #bits in result_type
  , ullong_bits=  h_types::ullong_bits               ///< #bits in ULLONG
  , bits_mod=     ullong_bits % size_t_bits
  , bits_div=     ullong_bits / size_t_bits + ( bits_mod > 0 )
  };

  enum : bool { easy_bits= ( size_t_bits >= ullong_bits )  };

  result_type
  operator()( array_t const & arr ) const noexcept
  {
    if( n_ullong == 0 ) return 0;
    if( n_ullong == 1 )
    {
      if( easy_bits ) return arr[0];
      return to_result_t( arr[0] );
    } // if n_ullong == 1

    return cmpsd_hash( arr );
  }

  result_type
  to_result_t( ULLONG a ) const noexcept
  {
    result_type ret_val= 0;
    size_t      shft=    0;
    for( size_t c= 0; c < bits_div; ++c, shft += size_t_bits )
    {
      auto const crrnt= result_type( a >> shft );
      do_combine( ret_val, crrnt, c );
    }
    return ret_val;
  } // to_result_t

  result_type
  cmpsd_hash( array_t const & arr ) const noexcept
  {
    result_type ret_val= 0;

    for( size_t c= 0; c < n_ullong; ++c )
    {
      auto const crrnt= easy_bits
                      ? result_type(arr[c]) : to_result_t(arr[c]);
      do_combine( ret_val, crrnt, c );
    }

    return ret_val;
  } // cmpsd_hash

  void
  do_combine( result_type &r, result_type crrnt, size_t cnt ) const noexcept
  {
    crrnt += cnt;
    auto const n_rot= cnt % size_t_bits;
    if( n_rot > 0 )
              crrnt= ( crrnt << n_rot ) | ( crrnt >> (size_t_bits-n_rot) );

    r ^= crrnt;
  } // do_combine
}; // struct hash_impl




} } // namespace Bitset2::detail




#endif // BITSET2_HASH_CB_HPP

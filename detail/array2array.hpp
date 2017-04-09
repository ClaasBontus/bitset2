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

#ifndef BITSET2_ARRAY2ARRAY_CB_HPP
#define BITSET2_ARRAY2ARRAY_CB_HPP


#include "h_types.hpp"


namespace Bitset2
{
namespace detail
{


/// \brief Convert std::array<Tsrc,src_n> to std::array<Ttrgt,trgt_n>
template<size_t trgt_n, size_t src_n, class Ttrgt, class Tsrc>
struct array2array
{
  using h_t_trgt=           h_types<Ttrgt>;
  using h_t_src=            h_types<Tsrc>;
  using trgt_array_t=       typename h_t_trgt::template array_t<trgt_n>;
  using src_array_t=        typename h_t_src:: template array_t<src_n>;

  enum : size_t
  { trgt_base_n_bits=   h_t_trgt::base_t_n_bits
  , src_base_n_bits=    h_t_src:: base_t_n_bits
  };

  enum : bool
  { small_to_large= ( src_base_n_bits < trgt_base_n_bits ) };

  enum : size_t
  { ts_div= small_to_large ? trgt_base_n_bits / src_base_n_bits
                           : src_base_n_bits  / trgt_base_n_bits };

  enum : Tsrc
  { h_all_set= Tsrc( Ttrgt(~Ttrgt(0)) ) };

  /// Applies pttrn to most significant entry in result
  constexpr
  trgt_array_t
  operator()( Ttrgt pttrn, src_array_t const &src ) const noexcept
  {
    static_assert( ( small_to_large && trgt_base_n_bits % src_base_n_bits == 0)
                || (!small_to_large && src_base_n_bits % trgt_base_n_bits == 0),
                   "Conversion between arrays of these types not supported" );
    return small_to_large
      ? conv_small_to_large( pttrn, src, std::make_index_sequence<trgt_n-1>() )
      : conv_large_to_small( pttrn, src, std::make_index_sequence<trgt_n-1>() );
  }


  template<size_t ... S1>
  constexpr
  trgt_array_t
  conv_small_to_large( Ttrgt                      pttrn,
                       src_array_t const          &src,
                       std::index_sequence<S1...> ) const noexcept
  {
    return {{ get_from_smaller( S1, src, S1 * ts_div )...,
              Ttrgt(   get_from_smaller( trgt_n-1, src, (trgt_n-1) * ts_div )
                     & pttrn ) }};
  }

  template<size_t ... S1>
  constexpr
  trgt_array_t
  conv_large_to_small( Ttrgt                      pttrn,
                       src_array_t const          &src,
                       std::index_sequence<S1...> ) const noexcept
  {
    return
      {{ get_from_larger( S1 / ts_div, S1 % ts_div, src )...,
         Ttrgt( get_from_larger((trgt_n-1) / ts_div, (trgt_n-1) % ts_div, src)
                & pttrn ) }};
  }

  constexpr
  Ttrgt
  get_from_smaller( size_t             trgt_idx,
                    src_array_t const  &src,
                    size_t             src_idx,
                    size_t             src_ct= 0,
                    Ttrgt              so_far= Ttrgt(0) ) const noexcept
  {
    return ( src_ct >= ts_div || src_idx >= src_n )
      ? so_far
      : get_from_smaller( trgt_idx, src, src_idx + 1, src_ct + 1,
                          so_far | Ttrgt(    Ttrgt(src[src_idx])
                                          << (src_base_n_bits * src_ct) ) );
  }

  constexpr
  Ttrgt
  get_from_larger( size_t             div_val,
                   size_t             mod_val,
                   src_array_t const  &src )  const noexcept
  {
    return ( div_val >= src_n ) ? Ttrgt(0)
      : Ttrgt(Tsrc( src[div_val] >> (mod_val*trgt_base_n_bits) ) & h_all_set );
  }
}; // struct array2array



} // namespace detail
} // namespace Bitset2



#endif // BITSET2_ARRAY2ARRAY_CB_HPP

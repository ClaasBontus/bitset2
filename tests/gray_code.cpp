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


#include "../bitset2.hpp"
#include <cassert>
#include <iostream>


template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
binary_to_gray( Bitset2::bitset2<N,T> const &bs )
{ return bs ^ (bs >> 1); }


template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
gray_to_binary( Bitset2::bitset2<N,T> bs )
{
  Bitset2::bitset2<N,T>   mask= bs >> 1;
  for( ; !mask.none(); mask >>= 1 )  bs ^= mask;
  return bs;
} // gray_to_binary


int main()
{
  using ULLONG= unsigned long long;
  constexpr std::array<ULLONG,2>  arr_01a{{ 0xFEFDFCFBFAF9F8F7ull, 1ull }};
  constexpr Bitset2::bitset2<129> bs_01a{ arr_01a };
  constexpr auto                  gray_01a= binary_to_gray( bs_01a );
  constexpr auto                  bin_01a=  gray_to_binary( gray_01a );

  static_assert( bs_01a == bin_01a );

  std::cout << bs_01a << '\n' << gray_01a << '\n';

  return 0;
} // main

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


#include "ullong2array.hpp"
#include "array2u_long_t.hpp"
#include <iostream>
#include <cstdint>
#include <cassert>


int main()
{
  using ULLONG= unsigned long long;

  auto const all_set= ~ULLONG(0);

  using namespace Bitset2::detail;

  using a2l_65ll= Bitset2::detail::array2u_long_t<65,ULLONG,ULLONG>;

  auto const a1= ullong2array<18,uint32_t>()( all_set );
  std::array<uint32_t,1> const expt1{{ uint32_t((1ull << 18) - 1) }};
  size_t ct= 0;
  for( auto v: a1 )
  {
    std::cout << v << ",  ";
    assert( v == expt1[ct] );  ct++;
  }
  std::cout << "\n";
  assert( ct == 1 );


  auto const a2= ullong2array<34,uint32_t>()( all_set );
  std::array<uint32_t,2> const
       expt2{{ uint32_t(~uint32_t(0)), uint32_t((1ull << 2) - 1) }};
  ct= 0;
  for( auto v: a2 )
  {
    std::cout << v << ",  ";
    assert( v == expt2[ct] );  ct++;
  }
  std::cout << "\n";
  assert( ct == 2 );


  auto const a3= ullong2array<18,uint16_t>()( all_set );
  std::array<uint32_t,2> const
       expt3{{ uint16_t(~uint16_t(0)), uint16_t((1ull << 2) - 1) }};
  ct= 0;
  for( auto v: a3 )
  {
    std::cout << v << ",  ";
    assert( v == expt3[ct] );  ct++;
  }
  std::cout << "\n";
  assert( ct == 2 );


  auto const a4= ullong2array<18,uint8_t>()( all_set );
  std::array<uint8_t,3> const
    expt4{{uint8_t(~uint8_t(0)),uint8_t(~uint8_t(0)),uint8_t((1ull << 2) - 1)}};
  ct= 0;
  for( auto v: a4 )
  {
    std::cout << int(v) << ",  ";
    assert( v == expt4[ct] );  ct++;
  }
  std::cout << "\n";
  assert( ct == 3 );


  auto const a5= ullong2array<18,uint64_t>()( all_set );
  std::array<uint64_t,1> const expt5{{ uint64_t((1ull << 18) - 1) }};
  ct= 0;
  for( auto v: a5 )
  {
    std::cout << v << ",  ";
    assert( v == expt5[ct] );  ct++;
  }
  std::cout << "\n";
  assert( ct == 1 );


  auto const a6= ullong2array<34,uint64_t>()( all_set );
  std::array<uint64_t,1> const expt6{{ uint64_t((1ull << 34) - 1) }};
  ct= 0;
  for( auto v: a6 )
  {
    std::cout << v << ",  ";
    assert( v == expt6[ct] );  ct++;
  }
  std::cout << "\n";
  assert( ct == 1 );


  auto const a7= ullong2array<66,uint64_t>()( all_set );
  std::array<uint64_t,2> const
       expt7{{ uint64_t(~uint64_t(0)), uint64_t(0) }};
  ct= 0;
  for( auto v: a7 )
  {
    std::cout << v << ",  ";
    assert( v == expt7[ct] );  ct++;
  }
  std::cout << "\n";
  assert( ct == 2 );
  

  a2l_65ll::array_t  arr1{{ all_set, 0ull }};

  std::cout << '\n' << a2l_65ll::i_pttrn << '\n';
  std::cout << a2l_65ll::h_pttrn << '\n';
  std::cout << a2l_65ll::allset << '\n';
  std::cout << a2l_65ll::n_array << '\n';
  std::cout << a2l_65ll::use_vals << '\n';
  std::cout << a2l_65ll().check_overflow( arr1 ) << '\n';
}

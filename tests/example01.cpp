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

#include <iostream>
#include <array>
#include <cassert>
#include "bitset2.hpp"

template<size_t n_bits>
using BS2=    Bitset2::bitset2<n_bits>;
using ULLONG= typename BS2<1>::ULLONG;

int main()
{
  constexpr std::array<ULLONG,2>  ar1{{ ~(0ull), 0xFEDCBAull }};
  constexpr BS2<128>              b1{ ar1 };
  constexpr auto                  b1_add=  b1 + b1;
  constexpr auto                  b1_shft= b1 << 1; // binary shift
  static_assert( b1_add == b1_shft, "" );

  std::cout << b1.to_hex_string()       // 0000000000fedcbaffffffffffffffff
            << "\n"
            << b1_add.to_hex_string()   // 0000000001fdb975fffffffffffffffe
            << "\n";

  BS2<12>   b2;
  for( size_t c= 0; c < 12; c += 2 ) b2[c]= true;
  auto      b3= ~b2;
  std::cout << b2 << "\n";         // 010101010101
  std::cout << b2.flip() << "\n";  // 101010101010
  assert( b2 == b3 );

  BS2<7> const   b4{ "1110000" };
  auto const     b5= Bitset2::rotate_left( b4, 3 );
  std::cout << b4 << "\n"     // 1110000
            << b5 << "\n";    // 0000111
} // main

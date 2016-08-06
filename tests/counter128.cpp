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
#include "bitset2.hpp"


int main()
{
  Bitset2::bitset2<128> c;
  constexpr Bitset2::bitset2<128> tst{ 0xFFFFFFFFull };

  for( ;; ++c )
  {
    if( ( c & tst) == tst )
      std::cout << c.to_hex_string() << "\n";
  }
} // main

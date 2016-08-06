# bitset2: bitset improved

Bitset2::bitset2 is a header only library. It provides the same functionality as [std::bitset](http://en.cppreference.com/w/cpp/utility/bitset) with the
following extentions/changes. Focus was set on having as many functions
implemented as [constexpr](http://en.cppreference.com/w/cpp/language/constexpr)
as possible.
* Copy and move constructors are specified constexpr.
* Additional constexpr constructor `bitset2( std::array const & )`.
* Conversion from and to `std::bitset`.
* Operators implemented as constexpr are `~`, `==`, `!=`, `|`, `&`, `^`, `<<` (shift left), `>>` (shift right), `[]` (bit access).
* Functions implemented as constexpr are `test`, `none`, `any`, `all`, `count`, `to_ulong`, `to_ullong`.
* Additional constexpr operator `+` for adding two bitset2 objects.
* Additional operators `++`, `--`, `+=`.
* Additional constexpr functions `rotate_left` and `rotate_right` for binary rotations.
* Additional member functions `rotate_left` and `rotate_right`.
* Additional member function `to_hex_string()`.
* Additional constexpr function `convert_to<n>` for converting an *m*-bit bitset2 into an *n*-bit bitset2.

## Example
```.cpp
#include <iostream>
#include <array>
#include <cassert>
#include "bitset2.hpp"

template<size_t n_bits>
using BS2=    Bitset2::bitset2<n_bits>;
using ULLONG= unsigned long long;

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
```

## Trivia
The following code shows a counter based on a 128-bit integer. If the
counter gets incremented once at each nanosecond, you have to wait for
overflow for *only* [1.078 * 10<sup>22</sup> years](http://www.wolframalpha.com/input/?i=2%5E128+nanoseconds).
```.cpp
Bitset2::bitset2<128> c;
for( ;; ++c ) {}
```

## Caveats
* bitset2 requires a C++14 compliant compiler.
* Tested with gcc 4.9 and 5.3 and clang 3.7.

# bitset2: bitset improved

Bitset2::bitset2 is a header only library. It provides the same functionality as [std::bitset](http://en.cppreference.com/w/cpp/utility/bitset) with the
following extentions/changes. Focus was set on having as many functions
implemented as [constexpr](http://en.cppreference.com/w/cpp/language/constexpr)
as possible.
* Copy and move constructors are specified constexpr.
* Additional constexpr constructor `bitset2( std::array<ULLONG,N> const & )`.
* Conversion from and to `std::bitset`.
* Operators implemented as constexpr are `~`, `==`, `!=`, `|`, `&`, `^`, `<<` (shift left), `>>` (shift right), `[]` (bit access).
* Functions implemented as constexpr are `test`, `none`, `any`, `all`, `count`, `to_ulong`, `to_ullong`.
* Additional constexpr operator `+` for adding two bitset2 objects.
* Additional operators `++`, `--`, `+=`.
* Additional constexpr operators `<`, `>`, `<=`, `>=`.
* Additional constexpr functions `rotate_left` and `rotate_right` for binary rotations.
* Additional member functions `rotate_left` and `rotate_right`.
* Additional member function `to_hex_string()`.
* Additional constexpr function `convert_to<n>` for converting an *m*-bit bitset2 into an *n*-bit bitset2.
* Member function `data()` gives read access to the underlying `array<ULLONG,N>`. Here element with index zero is the least significant word.
* Additional functions `zip_fold_and` and `zip_fold_or`. See below for details.

## Example
```.cpp
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
```

## Trivia
The following code shows a counter based on a 128-bit integer. If the
counter gets incremented once at each nanosecond, you have to wait for
overflow for *only* [1.078 * 10<sup>22</sup> years](http://www.wolframalpha.com/input/?i=2%5E128+nanoseconds).
```.cpp
Bitset2::bitset2<128> c;
for( ;; ++c ) {}
```

## zip\_fold\_&ast;
Functions `zip_fold_and` and `zip_fold_or` are similar to
[`std::inner_product(...,BinOp1 op1,BinOp2 op2)`](http://en.cppreference.com/w/cpp/algorithm/inner_product)
with `op1` set to `&&` and `||`, resp. They are mapped over the underlying
`std::array<ULLONG,n>`s. `zip_fold_*` will
[short-circuit](http://en.cppreference.com/w/cpp/language/operator_logical)
if possible, which can
result in performance advantages. These functions expect two `bitset2`s and
a functional object `f`. The latter must accept two `ULLONG`s and return a `bool`.
In other words, `zip_fold_and` returns `true` if `f` returns `true` for each
pair of `ULLONG`s, while `zip_fold_or` returns `true` if `f` returns `true` for
at least one pair of `ULLONG`s.

For instance `is_subset_of` as proposed in [p0125r0](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0125r0.html)
can be implemented as follows.
```.cpp
template<size_t N>
bool
is_subset_of( Bitset2::bitset2<N> const &bs1, Bitset2::bitset2<N> const &bs2 )
{
  using ULLONG= typename Bitset2::bitset2<N>::ULLONG;
  return Bitset2::zip_fold_and( bs1, bs2,
                                []( ULLONG v1, ULLONG v2 ) noexcept
                                  // Any bit unset in v2 must not be set in v1
                                  { return (v1 & ~v2) == 0; } );
}

Bitset2::bitset2<7>    b7_a( "1000101" );
Bitset2::bitset2<7>    b7_b( "1010101" );
assert( is_subset_of( b7_a, b7_b) );
```

Similarly an `unequal` function can be defined as
```.cpp
template<size_t N>
bool
unequal( Bitset2::bitset2<N> const &bs1, Bitset2::bitset2<N> const &bs2 )
{
  using ULLONG= typename Bitset2::bitset2<N>::ULLONG;
  return Bitset2::zip_fold_or( bs1, bs2,
                               []( ULLONG v1, ULLONG v2 ) noexcept
                                 { return v1 != v2; } );
}
```

## Caveats
* bitset2 requires a C++14 compliant compiler.
* Tested with gcc 4.9 and 5.3 and clang 3.7.
* Memory footprint is not optimized for small bit-sizes since data are stored in a `std::array<ULLONG,n>`.

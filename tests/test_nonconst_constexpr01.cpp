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
#include <iostream>
#include <array>


template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
ce_op( Bitset2::bitset2<N,T> const &bs, size_t n, size_t op_t )
{
  auto ret_val= bs;
  for( size_t c= 0; c < n; c++ )
  {
    switch( op_t )
    {
      case 1: ++ret_val; break;
      case 2: ret_val++; break;
      case 3: --ret_val; break;
      case 4: ret_val--; break;
    }
  }
  return ret_val;
} // ce_op


template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
ce_opequal( Bitset2::bitset2<N,T> const &bs1,
            Bitset2::bitset2<N,T> const &bs2,
            size_t                      op_t )
{
  auto ret_val= bs1;

  switch( op_t )
  {
      case 1: ret_val += bs2; break;
      case 2: ret_val |= bs2; break;
      case 3: ret_val &= bs2; break;
      case 4: ret_val ^= bs2; break;
      case 5: ret_val.difference( bs2 ); break;
  }
  return ret_val;
} // ce_opequal


template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
ce_shftequal( Bitset2::bitset2<N,T> const &bs1,
              size_t                      shft,
              size_t                      op_t )
{
  auto ret_val= bs1;

  switch( op_t )
  {
      case 1: ret_val <<= shft; break;
      case 2: ret_val >>= shft; break;
      case 3: ret_val.rotate_left(  shft ); break;
      case 4: ret_val.rotate_right( shft ); break;
  }
  return ret_val;
} // ce_shftequal


template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
ce_sme_fncs( Bitset2::bitset2<N,T> const &bs1 )
{
  auto ret_val= bs1;
  Bitset2::bitset2<N,T> b2;
  b2.set();
  b2.set( 12, false );
  b2.test_set( 15, false );

  ret_val.reverse();
  ret_val.complement2();
  ret_val ^= b2;

  ret_val.flip();
  ret_val.flip( 42 );
  ret_val.flip();

  return ret_val;
} // ce_sme_fncs


int main()
{
  using ULLONG= unsigned long long;
  using namespace Bitset2;

  constexpr std::array<ULLONG,2>  s_arr_01a{{ 0xFFFFFFFFFFFFFFFFull, 1ull }};
  constexpr std::array<ULLONG,2>  s_arr_01b{{ 5ull, 0ull }};
  constexpr std::array<ULLONG,2>  bit12{{ 1ull << 12, 0ull }};
  constexpr std::array<ULLONG,2>  bit15{{ 1ull << 15, 0ull }};
  constexpr std::array<ULLONG,2>  bit42{{ 1ull << 42, 0ull }};
  constexpr bitset2<129>          zero;
  constexpr bitset2<129>          all_set= ~zero;
  constexpr bitset2<129>          sme_set1=
                                  all_set ^ bitset2<129>{ bit12 } ^ bitset2<129>{ bit15 };
  constexpr bitset2<129>          b_from_s_arr01a{ s_arr_01a };
  constexpr bitset2<129>          b_from_s_arr01b{ s_arr_01b };
  constexpr auto                  minus_s_arr_01b= complement2( b_from_s_arr01b );
  constexpr auto                  add_01a_b= b_from_s_arr01a + b_from_s_arr01b;
  constexpr auto                  sub_01a_b= b_from_s_arr01a + minus_s_arr_01b;
  constexpr auto                  inc_01a_5= ce_op( b_from_s_arr01a, 5, 1 );
  constexpr auto                  inc_01b_5= ce_op( b_from_s_arr01a, 5, 2 );
  constexpr auto                  dec_01c_5= ce_op( b_from_s_arr01a, 5, 3 );
  constexpr auto                  dec_01d_5= ce_op( b_from_s_arr01a, 5, 4 );

  constexpr auto                  or_01a_b=  b_from_s_arr01a | b_from_s_arr01b;
  constexpr auto                  and_01a_b= b_from_s_arr01a & b_from_s_arr01b;
  constexpr auto                  xor_01a_b= b_from_s_arr01a ^ b_from_s_arr01b;
  constexpr auto                  sdi_01a_b= difference( b_from_s_arr01a, b_from_s_arr01b );
  constexpr auto                  shl4_01a=  b_from_s_arr01a << 4;
  constexpr auto                  shr4_01a=  b_from_s_arr01a >> 4;
  constexpr auto                  rol4_01a=  rotate_left(  b_from_s_arr01a, 4 );
  constexpr auto                  ror4_01a=  rotate_right( b_from_s_arr01a, 4 );

  constexpr auto                  rev_01a=   reverse( b_from_s_arr01a );
  constexpr auto                  cpl2_01a=  complement2( rev_01a );
  constexpr auto                  xor12_01a= cpl2_01a ^ sme_set1;
  constexpr auto                  xor42_01a= xor12_01a ^ bitset2<129>{ bit42 };

  constexpr auto                  pe1= ce_opequal( b_from_s_arr01a, b_from_s_arr01b, 1 );
  constexpr auto                  oe1= ce_opequal( b_from_s_arr01a, b_from_s_arr01b, 2 );
  constexpr auto                  ae1= ce_opequal( b_from_s_arr01a, b_from_s_arr01b, 3 );
  constexpr auto                  xe1= ce_opequal( b_from_s_arr01a, b_from_s_arr01b, 4 );
  constexpr auto                  de1= ce_opequal( b_from_s_arr01a, b_from_s_arr01b, 5 );

  constexpr auto                  sl4_1= ce_shftequal( b_from_s_arr01a, 4, 1 );
  constexpr auto                  sr4_1= ce_shftequal( b_from_s_arr01a, 4, 2 );
  constexpr auto                  rl4_1= ce_shftequal( b_from_s_arr01a, 4, 3 );
  constexpr auto                  rr4_1= ce_shftequal( b_from_s_arr01a, 4, 4 );
  constexpr auto                  smf_a= ce_sme_fncs( b_from_s_arr01a );

  static_assert( add_01a_b == inc_01a_5, "" );
  static_assert( add_01a_b == inc_01b_5, "" );
  static_assert( sub_01a_b == dec_01c_5, "" );
  static_assert( sub_01a_b == dec_01d_5, "" );
  static_assert( add_01a_b  == pe1, "" );
  static_assert( or_01a_b   == oe1, "" );
  static_assert( and_01a_b  == ae1, "" );
  static_assert( xor_01a_b  == xe1, "" );
  static_assert( sdi_01a_b  == de1, "" );
  static_assert( shl4_01a   == sl4_1, "" );
  static_assert( shr4_01a   == sr4_1, "" );
  static_assert( rol4_01a   == rl4_1, "" );
  static_assert( ror4_01a   == rr4_1, "" );
  static_assert( xor42_01a  == smf_a, "" );

  std::cout << inc_01a_5 << '\n' << inc_01b_5.to_hex_string() << "\n";
  std::cout << sub_01a_b << '\n' << dec_01c_5.to_hex_string() << "\n";

  return 0;
} // main

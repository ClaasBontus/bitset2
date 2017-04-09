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
#include <sstream>
#include <cassert>
#include "bitset2.hpp"


template<size_t N,class T>
bool
is_subset_of( Bitset2::bitset2<N,T> const &bs1, Bitset2::bitset2<N,T> const &bs2 )
{
  using base_t= T;
  return Bitset2::zip_fold_and( bs1, bs2,
                                []( base_t v1, base_t v2 ) noexcept
                                  { return (v1 & ~v2) == 0; } );
} // is_subset_of


template<size_t N,class T>
bool
unequal( Bitset2::bitset2<N,T> const &bs1, Bitset2::bitset2<N,T> const &bs2 )
{
  using base_t= T;
  return Bitset2::zip_fold_or( bs1, bs2,
                               []( base_t v1, base_t v2 ) noexcept
                                 { return v1 != v2; } );
} // unequal



int main()
{
  using namespace Bitset2;
  using ULLONG= unsigned long long;

  constexpr bitset2<32>   b1;
  constexpr auto          v1= b1.to_ullong();
  constexpr auto          b1_n= ~b1;
  constexpr bool          b1_n_none= b1_n.none();
  constexpr bool          b1_n_all= b1_n.all();
  constexpr bool          b1_n_any= b1_n.any();

  bitset2<63>             b_63;
  bitset2<64>             b_64;
  bitset2<65>             b_65;
  auto                    b1a= b1;
  b1a.set();
  b_63.set();
  b_64.set();
  b_65.set();

  std::bitset<65>         sb1;
  auto                    sb2= std::bitset<32>( b1 );
  sb1[64]= 1;

  bitset2<65>             b2( sb1 );

  std::cout << v1 << "\n"
            << b2 << "\n"
            << sb2 << "\n"
            << b1_n << "\n"
            << "b1.none()= " << b1.none() << "  "
            << "b1.any()= " << b1.any() << "  "
            << "b1.all()= " << b1.all() << "\n"
            << "b1a.none()= " << b1a.none() << "  "
            << "b1a.any()= " << b1a.any() << "  "
            << "b1a.all()= " << b1a.all() << "\n"
            << "b2.none()= " << b2.none() << "  "
            << "b2.any()= " << b2.any() << "  "
            << "b2.all()= " << b2.all() << "\n"
            << "b1_n.none()= " << b1_n_none << "  "
            << "b1_n.any()= " << b1_n_any << "  "
            << "b1_n.all()= " << b1_n_all << "\n"
            << "b_63.all()= " << b_63.all() << "  "
            << "b_64.all()= " << b_64.all() << "  "
            << "b_65.all()= " << b_65.all() << "\n";
  std::cout << "b_63.flip()= " << b_63.flip() << "\n"
            << "b_64.flip()= " << b_64.flip() << "\n"
            << "b_65.flip()= " << b_65.flip() << "\n";
  std::cout << "b_63.flip()= " << b_63.flip() << "\n"
            << "b_64.flip()= " << b_64.flip() << "\n"
            << "b_65.flip()= " << b_65.flip() << "\n";
  std::cout << "b_63.reset()= " << b_63.reset() << "\n"
            << "b_64.reset()= " << b_64.reset() << "\n"
            << "b_65.reset()= " << b_65.reset() << "\n";

  detail::bit_chars<4,ULLONG>   bc1;
  detail::bit_chars<64,ULLONG>  bc2;
  detail::bit_chars<65,ULLONG>  bc3;
  detail::bit_chars<63,ULLONG>  bc4;

  bitset2<64>  lbp1( bc1.low_bit_pattern );
  bitset2<64>  lbp2( bc2.low_bit_pattern );
  bitset2<64>  lbp3( bc3.low_bit_pattern );
  bitset2<64>  lbp4( bc4.low_bit_pattern );

  std::cout << bc1.low_bit_pattern << "  " << bc1.mod_val << "\n"
            << "  " << lbp1 << "\n"
            << bc2.low_bit_pattern << "  " << bc2.mod_val << "\n"
            << "  " << lbp2 << "\n"
            << bc3.low_bit_pattern << "  " << bc3.mod_val << "\n"
            << "  " << lbp3 << "\n"
            << bc4.low_bit_pattern << "  " << bc4.mod_val << "\n"
            << "  " << lbp4 << "\n";

  constexpr
  std::array<unsigned long long,2>  ar1{{ ~(0ull), 1 }};

  constexpr
  auto s_ar1= detail::array_ops<128,ULLONG>( 63 ).shift_left( ar1 );

  std::cout << "\n" << ar1[1] << "  " << ar1[0] << "\n";
  std::cout << "\n" << s_ar1[1] << "  " << s_ar1[0] << "\n";

  bitset2<63>  bc5{ 1ull };
  bitset2<64>  bc6{ 1ull };
  bitset2<65>  bc7{ 1ull };
  constexpr bitset2<63>  bc5a{ 1ull };
  constexpr bitset2<64>  bc6a{ 1ull };
  constexpr bitset2<65>  bc7a{ 1ull };
  constexpr bitset2<127>  bc8a{  1ull };
  constexpr bitset2<128>  bc9a{  1ull };
  constexpr bitset2<129>  bc10a{ 1ull };
  constexpr auto bc5b= bc5a << 62;
  constexpr auto bc6b= bc6a << 63;
  constexpr auto bc7b= bc7a << 64;
  constexpr auto bc8b= bc8a  << 62;
  constexpr auto bc8c= bc8a  << 63;
  constexpr auto bc8d= bc8a  << 64;
  constexpr auto bc9b= bc9a  << 62;
  constexpr auto bc9c= bc9a  << 63;
  constexpr auto bc9d= bc9a  << 64;
  constexpr auto bc10b= bc10a << 62;
  constexpr auto bc10c= bc10a << 63;
  constexpr auto bc10d= bc10a << 64;
  std::cout << "  " << bc8b << "\n  " << bc8c << "\n  " << bc8d << "\n";
  std::cout << " " << bc9b << "\n " << bc9c << "\n " << bc9d << "\n";
  std::cout << bc10b << "\n" << bc10c << "\n" << bc10d << "\n";
  std::cout << bc5b << " " << bc6b << " " << bc7b << "\n";
  for( size_t c= 0; c < 66; ++c )
  {
    std::cout << bc5 << " " << bc6 << " " << bc7 << "\n";
    bc5 <<= 1;    bc6 <<= 1;   bc7 <<= 1;
  }

  std::cout << "\n";
  constexpr auto bc5c= bc5b >> 62;
  constexpr auto bc6c= bc6b >> 63;
  constexpr auto bc7c= bc7b >> 64;
  std::cout << bc5c << " " << bc6c << " " << bc7c << "\n";

  constexpr auto bc5d= bc5a | bc5b;
  constexpr auto bc5e= bc5d & bc5a;
  constexpr auto bc5f= bc5d & bc5b;
  constexpr auto bc5g= bc5a ^ bc5b;
  constexpr auto bc5h= bc5d ^ bc5a;
  constexpr auto bc5i= bc5d ^ bc5b;
  constexpr auto bc5j= bc5a << 1;
  std::cout << "\n" << bc5a << " |\n" << bc5b << " =\n" << bc5d << "\n";
  std::cout << "\n" << bc5d << " &\n" << bc5a << " =\n" << bc5e << "\n";
  std::cout << "\n" << bc5d << " &\n" << bc5b << " =\n" << bc5f << "\n";
  std::cout << "\n" << bc5a << " ^\n" << bc5b << " =\n" << bc5g << "\n";
  std::cout << "\n" << bc5d << " ^\n" << bc5a << " =\n" << bc5h << "\n";
  std::cout << "\n" << bc5d << " ^\n" << bc5b << " =\n" << bc5i << "\n";
  std::cout << "\n" << bc5a << " << 1 = " << bc5j << "\n";

  constexpr auto bc7d= bc7a | bc7b;
  constexpr auto bc7e= bc7d & bc7a;
  constexpr auto bc7f= bc7d & bc7b;
  constexpr auto bc7g= bc7a ^ bc7b;
  constexpr auto bc7h= bc7d ^ bc7a;
  constexpr auto bc7i= bc7d ^ bc7b;
  std::cout << "\n" << bc7a << " |\n" << bc7b << " =\n" << bc7d << "\n";
  std::cout << "\n" << bc7d << " &\n" << bc7a << " =\n" << bc7e << "\n";
  std::cout << "\n" << bc7d << " &\n" << bc7b << " =\n" << bc7f << "\n";
  std::cout << "\n" << bc7a << " ^\n" << bc7b << " =\n" << bc7g << "\n";
  std::cout << "\n" << bc7d << " ^\n" << bc7a << " =\n" << bc7h << "\n";
  std::cout << "\n" << bc7d << " ^\n" << bc7b << " =\n" << bc7i << "\n";

  constexpr auto bc5a_f= ~bc5a;
  constexpr auto bc5d_f= ~bc5d;
  constexpr auto bc7a_f= ~bc7a;
  constexpr auto bc7d_f= ~bc7d;
  std::cout << "\n~" << bc5a << " =\n " << bc5a_f << "\n";
  std::cout << "\n~" << bc5d << " =\n " << bc5d_f << "\n";
  std::cout << "\n~" << bc7a << " =\n " << bc7a_f << "\n";
  std::cout << "\n~" << bc7d << " =\n " << bc7d_f << "\n";
  std::cout << "\n";
  bitset2<65>  bc11a{ 1ull };
  bitset2<65>  bc11b{ 1ull };
  bc11b <<= 64;
  bc11b |= ( bc11a << 1 );
  std::cout << bc11b << "\n";
  bc11b >>= 1;
  std::cout << bc11b << "\n";
  std::cout << bc11b.to_string( '.', 'x' ) << "\n";

  constexpr auto n_5a=   bc5a.count();
  constexpr auto n_5a_f= bc5a_f.count();
  constexpr auto n_7a=   bc7a.count();
  constexpr auto n_7a_f= bc7a_f.count();
  std::cout << "count( " << bc5a << " )= " << n_5a << "\n";
  std::cout << "count( " << bc5a_f << " )= " << n_5a_f << "\n";
  std::cout << "count( " << bc7a << " )= " << n_7a << "\n";
  std::cout << "count( " << bc7a_f << " )= " << n_7a_f << "\n";
  std::cout << "\n";
  b_63.reset();
  b_64.reset();
  b_65.reset();
  auto b_63a= b_63;
  auto b_64a= b_64;
  auto b_65a= b_65;
  for( size_t c= 0; c < 64; c += 2 )
  {
    if( c < 63 ) b_63[c]= true;
    if( c < 64 ) b_64[c]= true;
    if( c < 65 ) b_65[c]= true;
    b_63a[c/2]= true;  b_64a[c/2]= true;  b_65a[c/2]= true;
  }
  std::cout << "Hash values:\n";
  std::cout << "  " << b_63 << " " << std::hash<bitset2<63>>()( b_63 ) << "\n"
            << " "  << b_64 << " " << std::hash<bitset2<64>>()( b_64 ) << "\n"
            << b_65 << " " << std::hash<bitset2<65>>()( b_65 ) << "\n";

  bitset2<0>  b_0a, b_0b;
  constexpr auto bl_c7d= bc7d == bc7d;
  std::cout << "\n" << ( b_63 == b_63 ) << "  "
            << ( b_64 == b_64 ) << "  "
            << ( b_65 == b_65 ) << "\n"
            << ( b_63 != b_63a ) << "  "
            << ( b_64 != b_64a ) << "  "
            << ( b_65 != b_65a ) << "  "
            << bl_c7d  << "\n"
            << ( b_0a == b_0b ) << "\n";

  constexpr bitset2<65>  bs33a( 0xFFFFFFFFFFFFFFFFull );
  // constexpr auto         bs33b= ( bs33a << 1 );
  //constexpr
  auto         bs33a_v= bs33a.to_ulong();
  std::cout << bs33a << " == " << bs33a_v << " == " << bs33a.to_hex_string() << "\n";
  // auto                   bs33b_v= bs33b.to_ulong(); // throws
  // std::cout << bs33b << " == " << bs33b_v << "\n";

  bitset2<63>  bc2_63{ 0x700000000000000Eull };
  constexpr bitset2<63>  bc2_63a{ 0x700000000000000Eull };
  constexpr auto         bc2_63b= rotate_right( bc2_63a, 5 );
  bitset2<65>  bc2_65{ 0x700000000000000Eull };
  constexpr bitset2<65>  bc2_65a{ 0x700000000000000Eull };
  constexpr auto         bc2_65b= rotate_right( bc2_65a, 5 );
  std::cout << "               bc2_63=   " << bc2_63
            << " == " << bc2_63.to_hex_string()  << "\n";
  bc2_63.rotate_left( 5 );
  std::cout << "rot_left( bc2_63, 5) =   " << bc2_63 << "\n";
  std::cout << "rot_right( bc2_63a,5)=   " << bc2_63b << "\n";

  std::cout << "               bc2_65= " << bc2_65
            << " == " << bc2_65.to_hex_string()  << "\n";
  bc2_65.rotate_left( 5 );
  std::cout << "rot_left( bc2_65, 5) = " << bc2_65 << "\n";
  std::cout << "rot_right( bc2_65a,5)= " << bc2_65b << "\n";

  constexpr auto add_b5a= bc5a + bc5j;
  constexpr auto add_b5b= bc5a + bc5a;
  std::cout << bc5a << " +\n" << bc5j << " =\n" << add_b5a << "\n";
  std::cout << bc5a << " +\n" << bc5a << " =\n" << add_b5b << "\n";

  constexpr std::array<ULLONG,2>  arr_add_01a{{ 0xFFFFFFFFFFFFFFFFull, 0ull }};
  std::array<ULLONG,2>  arr_add_01ap{{ 0xFFFFFFFFFFFFFFFFull, 0ull }};
  constexpr std::array<ULLONG,2>  arr_add_01b{{ 0xFFFFFFFFFFFFFFFFull, 1ull }};
  constexpr bitset2<64>  b_a_01a( 0xFFFFFFFFFFFFFFFFull );
  constexpr bitset2<128> b_a_02a( arr_add_01a );
  bitset2<128> b_a_02ap( arr_add_01ap );
  constexpr bitset2<128> b_a_02b( arr_add_01b );

  std::cout << b_a_02ap.to_hex_string() << "\n";

  constexpr auto add_b_a_01a= b_a_01a + b_a_01a;
  constexpr auto add_b_a_02a= b_a_02a + b_a_02a;
  constexpr auto add_b_a_02b= b_a_02a + b_a_02b;
  constexpr auto add_b_a_02c= b_a_02b + b_a_02b;
  std::cout << b_a_01a << " +\n" << b_a_01a << " =\n" << add_b_a_01a << "\n\n";
  std::cout << b_a_02a << " +\n" << b_a_02a << " =\n" << add_b_a_02a << "\n\n";
  std::cout << b_a_02a << " +\n" << b_a_02b << " =\n" << add_b_a_02b << "\n\n";
  std::cout << b_a_02b << " +\n" << b_a_02b << " =\n" << add_b_a_02c << "\n\n";

  bitset2<65>  bc2_65c{ 0xF00000000000000Eull };
  std::cout << bc2_65c << " +\n" << bc2_65a << " =\n";
  bc2_65c += bc2_65a;
  std::cout << bc2_65c << "\n\n";

  bitset2<65>  bc2_65d{ 0xFFFFFFFFFFFFFFFEull };
  std::cout << "++" << bc2_65d << "=\n  ";
  std::cout << (++bc2_65d) << "\n";

  bitset2<65>  bc2_65e = bc2_65d << 1;

  std::cout << "++" << bc2_65d << "=\n  ";
  std::cout << (++bc2_65d) << "\n";

  std::cout << "++" << bc2_65e << "=\n  ";
  std::cout << (++bc2_65e) << "\n";
  std::cout << "  " << bc2_65e << "++ =\n  ";
  bc2_65e++;
  std::cout << bc2_65e << "\n";

  bitset2<65>  bc2_65f= bitset2<65>{1ull} << 64;
  bitset2<65>  bc2_65g;
  std::cout << "\n--" << bc2_65f << "=\n  ";
  std::cout << (--bc2_65f) << "\n";
  std::cout << "  " << bc2_65f << "-- =\n  ";
  bc2_65f--;
  std::cout << bc2_65f << "\n";
  std::cout << "--" << bc2_65g << "=\n  ";
  std::cout << (--bc2_65g) << "\n";

  std::string bit_string = "101110";
  std::istringstream bit_stream( bit_string );
  bitset2<3>  b_from_stream1;
  bitset2<3>  b_from_str2( bit_string, 3 );
  bitset2<4>  b_from_str3( bit_string.c_str() + 1, 3 );
  bit_stream >> b_from_stream1;
  std::cout << b_from_stream1
            << "= 0x" << b_from_stream1.to_hex_string() << '\n';
  std::cout << b_from_str2
            << "= 0x" << b_from_str2.to_hex_string() << '\n';
  std::cout << b_from_str3
            << "= 0x" << b_from_str3.to_hex_string() << '\n';


  constexpr std::array<ULLONG,1>  s_arr_01a{{ 1ull }};
  constexpr std::array<ULLONG,2>  s_arr_01b{{ 0xFFFFFFFFFFFFFFFFull, 1ull }};
  constexpr std::array<ULLONG,3>  s_arr_01c{{ 0x1ull, 0xFFFFFFFFFFFFFFFFull, 0x3ull }};
  constexpr std::array<ULLONG,4>  s_arr_01d{{ 0xEEEEEEEEEEEEEEEEull, 0xDull, 1ull, 0xFFFFFFFFFFFFFFFFull }};
  constexpr bitset2<129>          b_from_s_arr01a{ s_arr_01a };
  constexpr bitset2<129>          b_from_s_arr01b{ s_arr_01b };
  constexpr bitset2<129>          b_from_s_arr01c{ s_arr_01c };
  constexpr bitset2<129>          b_from_s_arr01d{ s_arr_01d };
  std::cout << "b_from_s_arr01a= " << b_from_s_arr01a.to_hex_string() << "\n";
  std::cout << "b_from_s_arr01b= " << b_from_s_arr01b.to_hex_string() << "\n";
  std::cout << "b_from_s_arr01c= " << b_from_s_arr01c.to_hex_string() << "\n";
  std::cout << "b_from_s_arr01d= " << b_from_s_arr01d.to_hex_string() << "\n";

  constexpr bitset2<24>      b24_empty{ 0ull };
  constexpr bitset2<24>      b24_full= ~b24_empty;
  constexpr bitset2<23>      b23_a=        convert_to<23>( b24_full );
  constexpr bitset2<25>      b25_a=        convert_to<25>( b24_full );
  constexpr auto             b24_full_ui8= convert_to<24,uint8_t>( b24_full );
  std::cout << "b24_full=     "  << b24_full << "\n";
  std::cout << "b23_a=         " << b23_a << "\n";
  std::cout << "b25_a=       "   << b25_a << "\n";
  std::cout << "b24_full_ui8= "  << b24_full_ui8 << '\n';

  bitset2<7>    b7_a( "1010101" );
  bitset2<7>    b7_b( "1000101" );
  bitset2<7>    b7_c( "1110101" );
  bitset2<7>    b7_d( "0110101" );
  bitset2<7>    b7_e( "1010101" );

  assert(  is_subset_of( b7_b, b7_a ) );
  assert( !is_subset_of( b7_c, b7_a ) );
  assert( !is_subset_of( b7_d, b7_a ) );
  assert( !is_subset_of( b7_a, b7_d ) );

  assert(  unequal( b7_a, b7_b ) );
  assert( !unequal( b7_e, b7_a ) );

  assert( b7_b <  b7_a );
  assert( b7_c >  b7_a );
  assert( b7_e <= b7_a );
  assert( b7_a >= b7_d );

  bitset2<2047>  b2047_a( 1ull );
  auto           b2047_b= b2047_a << 999;
  auto           b2047_c= b2047_a + b2047_b;
  auto           b2047_d= b2047_a << 1999;
  std::cout << "b2047_a= " << b2047_a.to_hex_string() << "\n"
            << "b2047_b= " << b2047_b.to_hex_string() << "\n"
            << "b2047_c= " << b2047_c.to_hex_string() << "\n";
  std::cout << "bitset2<2047>::n_array= " << bitset2<2047>::n_array << '\n';
  assert( unequal( b2047_a, b2047_b ) );
  assert( unequal( b2047_b, b2047_d ) );
  assert(  is_subset_of( b2047_a, b2047_c ) );
  assert( !is_subset_of( b2047_b, b2047_d ) );

  bitset2<16>  b16_a( "0000101000011111" );
  bitset2<16>  b16_b;
  hex_params<> hp1;
  hp1.aCh=           'A';
  hp1.leadingZeroes= false;
  hp1.prefix=        "0x";
  std::cout << '\n'
    << b16_a.to_hex_string() << '\n'                                    // 0a1f
    << b16_a.to_hex_string( hp1 )                                       // 0xA1F
    << '\n'
    << b16_b.to_hex_string() << '\n'                                    // 0000
    << b16_b.to_hex_string( hex_params<>{'0', 'a', false, false, "0X"}) // 0X
    << '\n';
} // main

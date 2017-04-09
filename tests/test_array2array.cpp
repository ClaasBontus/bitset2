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


#include "array2array.hpp"
#include "gen_randoms.hpp"
#include <iostream>
#include <cstdint>
#include <cassert>



constexpr size_t n_loops= 100000;



template<class T1,class T2>
void
fwd_bckwd()
{
  constexpr size_t  bits_T1= sizeof(T1) * CHAR_BIT;
  constexpr size_t  bits_T2= sizeof(T2) * CHAR_BIT;

  constexpr size_t  t_n1= 8,  s_n1= 1;
  constexpr size_t  t_n2= 24, s_n2= 3;

  using a2a_t1a= Bitset2::detail::array2array<t_n1,s_n1,T2,T1>;
  using a2a_t1b= Bitset2::detail::array2array<s_n1,t_n1,T1,T2>;
  using a2a_t2a= Bitset2::detail::array2array<t_n1,s_n1,T1,T2>;
  using a2a_t2b= Bitset2::detail::array2array<s_n1,t_n1,T2,T1>;

  using a2a_t3a= Bitset2::detail::array2array<t_n2,s_n2,T2,T1>;
  using a2a_t3b= Bitset2::detail::array2array<s_n2,t_n2,T1,T2>;
  using a2a_t4a= Bitset2::detail::array2array<t_n2,s_n2,T1,T2>;
  using a2a_t4b= Bitset2::detail::array2array<s_n2,t_n2,T2,T1>;

  gen_random_bitset2<s_n1*bits_T1,T1>  gen_rand1a;
  gen_random_bitset2<s_n1*bits_T2,T2>  gen_rand1b;

  gen_random_bitset2<s_n2*bits_T1-bits_T1/2,T1>  gen_rand2a;
  gen_random_bitset2<s_n2*bits_T2-bits_T2/2,T2>  gen_rand2b;

  for( size_t ct= 0; ct < n_loops; ++ct )
  {
    // if( (ct+1) % 1000 == 0 ) std::cout << (ct+1) << '\n';

    auto const bs1= gen_rand1a();
    auto const a1a= a2a_t1a()( ~T2(0), bs1.data() );
    auto const a1b= a2a_t1b()( ~T1(0), a1a );
    assert( bs1.data() == a1b );

    auto const bs2= gen_rand1b();
    auto const a2a= a2a_t2a()( ~T1(0), bs2.data() );
    auto const a2b= a2a_t2b()( ~T2(0), a2a );
    assert( bs2.data() == a2b );

    auto const bs3= gen_rand2a();
    auto const a3a= a2a_t3a()( ~T2(0), bs3.data() );
    auto const a3b= a2a_t3b()( T1(T1(~T1(0)) >> (bits_T1/2)), a3a );
    assert( bs3.data() == a3b );

    auto const bs4= gen_rand2b();
    auto const a4a= a2a_t4a()( ~T1(0), bs4.data() );
    auto const a4b= a2a_t4b()( T2(T2(~T2(0)) >> (bits_T2/2)), a4a );
    assert( bs4.data() == a4b );
  } // for ct
} // fwd_bckwd


int main()
{

  constexpr size_t  t_n1= 2, s_n1= 2;

  std::cout << Bitset2::detail::array2array<t_n1,s_n1,uint16_t,uint8_t>::h_all_set
            << "  "
            // << Bitset2::detail::array2array<t_n1,s_n1,uint8_t,uint16_t>::h_all_set
            // << "  "
            << Bitset2::detail::array2array<t_n1,s_n1,uint8_t,uint32_t>::h_all_set
            << "  "
            << Bitset2::detail::array2array<t_n1,s_n1,uint16_t,uint32_t>::h_all_set
            << "  "
            << Bitset2::detail::array2array<t_n1,s_n1,uint16_t,uint64_t>::h_all_set
            << "  "
            << Bitset2::detail::array2array<t_n1,s_n1,uint32_t,uint64_t>::h_all_set
            << '\n';

  std::cout << " 8 <-> 64\n";
  fwd_bckwd<uint8_t,uint64_t>();
  std::cout << "16 <-> 64\n";
  fwd_bckwd<uint16_t,uint64_t>();
  std::cout << "32 <-> 64\n";
  fwd_bckwd<uint32_t,uint64_t>();

  std::cout << "\n 8 <-> 32\n";
  fwd_bckwd<uint8_t,uint32_t>();
  std::cout << "16 <-> 32\n";
  fwd_bckwd<uint16_t,uint32_t>();

  std::cout << "\n 8 <-> 16\n";
  fwd_bckwd<uint8_t,uint16_t>();
} // main

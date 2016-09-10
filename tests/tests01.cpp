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
#include "gen_randoms.hpp"
#include <iostream>
#include <bitset>
#include <cassert>


#define TESTMANY(F) \
  F <63>(); \
  F <64>(); \
  F <65>(); \
  F <95>(); \
  F <96>(); \
  F <97>(); \
  F <127>(); \
  F <128>(); \
  F <129>(); \
  F <255>(); \
  F <256>(); \
  F <257>();


template<size_t N>
using t1= Bitset2::bitset2<N>;

template<size_t N>
using t2= std::bitset<N>;


constexpr size_t n_loops= 100000;

constexpr bool verbose= false;


template<size_t N>
struct dummy_add
{
  using ULLONG= unsigned long long;
  enum : size_t
  {   ull_bits= sizeof(ULLONG) * CHAR_BIT
    , div_val=  N / ull_bits
    , mod_val=  N % ull_bits
    , n_ull=    ( mod_val != 0 ) ? (div_val+1) : div_val
    , n_array=  ( N == 0 )       ? 1 : n_ull
  };
  //
  enum : ULLONG
  {   hgh_pattern=  (N==0) ? 0ull
                           : (mod_val==0) ? (~0ull)
                                          : ((~0ull) >> (ull_bits-mod_val))
  };
  //
  using array_t= std::array<ULLONG,n_array>;
  //
  array_t
  add( array_t const &a1, array_t const &a2 ) const
  {
    array_t  ret_val;
    ULLONG   crry= 0ull;

    for( size_t c= 0; c < n_ull; ++c )
    {
      auto const v= a1[c] + a2[c] + crry;
      if( v < a1[c] || v < a2[c] ) crry= 1ull;
      else                         crry= 0ull;
      ret_val[c]= v;
    }
    if( n_ull ) ret_val[n_ull-1] &= hgh_pattern;

    return ret_val;
  } // add
  //
  // Returns false if a1 != a2
  bool
  compare( array_t const &a1, array_t const &a2 )
  {
    for( size_t c= 0; c < n_array; ++c )
    {
      if( a1[c] != a2[c] ) return false;
    }
    return true;
  }
}; // struct dummy_add



template<size_t N>
void
test_any_all_none()
{
  std::cout << "Entering test_any_all_none N= " << N << "\n";

  t1<N> const empty1;
  t2<N> const empty2;
  auto  const full1= ~empty1;
  auto  const full2= ~empty2;

  auto const  empty1a= t1<N>( empty2 );
  auto const  empty2a= t2<N>( empty1 );
  auto const  full1a=  t1<N>( full2 );
  auto const  full2a=  t2<N>( full1 );

  assert(  empty1.none()  && !empty1.all()  && !empty1.any() );
  assert(  empty1a.none() && !empty1a.all() && !empty1a.any() );
  assert(  empty2a.none() && !empty2a.all() && !empty2a.any() );
  assert( !full1.none()   &&  full1.all()   &&  full1.any() );
  assert( !full1a.none()  &&  full1a.all()  &&  full1a.any() );
  assert( !full2a.none()  &&  full2a.all()  &&  full2a.any() );

  gen_random_bitset2<N>  gen_rand;
  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1= gen_rand();
    auto const  bs2= t2<N>( bs1 );
    auto const  bs1a= t1<N>( bs2 );

    if( verbose ) std::cout << bs1.to_hex_string() << "\t" << c << "\n";

    assert( bs1 == bs1a );
    assert( bs1.none() == bs2.none() );
    assert( bs1.all()  == bs2.all() );
    assert( bs1.any()  == bs2.any() );
  } // for c
} // test_any_all_none




template<size_t N>
void
test_set_count_size()
{
  std::cout << "Entering test_set_count_size N= " << N << "\n";

  t1<N> const empty1;
  assert( empty1.size() == N );

  gen_random_bitset2<N>  gen_rand;
  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1=  gen_rand();
    auto const  cnt1= bs1.count();
    size_t      n_set1= 0, n_set2= 0;
    for( size_t b_c= 0; b_c < N; ++b_c )
    {
      if( bs1.test( b_c ) ) ++n_set1;
      if( bs1[b_c] )        ++n_set2;
    }

    if( verbose ) std::cout << bs1.to_hex_string() << "\t" << n_set1
                            << "\t" << c << "\t" << bs1 << "\n";

    auto bs2= bs1;
    bs2.flip();
    auto const cnt2= bs2.count();

    assert( n_set1 == cnt1 );
    assert( n_set1 == n_set2 );
    assert( cnt2   == N - cnt1 );
  } // for c
} // test_set_count_size




template<size_t N>
void
test_set()
{
  std::cout << "Entering test_set N= " << N << "\n";

  gen_random_bitset2<N>  gen_rand;
  for( size_t c= 0; c < n_loops; ++c )
  {
    auto  bs1=  gen_rand();
    auto  bs2=  bs1;
    auto  bs3= t2<N>( bs1 );
    bool  flag= false;
    for( size_t b= 0; b < N; ++ b )
    {
      bool const bt1= bs1[b];
      bs1.set( b, flag );
      auto const bt2= bs2.test_set( b, flag);
      assert( bt1 == bt2 );
      bs3.set( b, flag );

      flag= !flag;
    }
    assert( bs1 == bs2 );
    assert( bs1 == t1<N>( bs3 ) );
  } // for c
} // test_set




template<size_t N>
void
test_rotate()
{
  std::cout << "Entering test_rotate N= " << N << "\n";

  gen_random_bitset2<N>  gen_rand;
  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1=  gen_rand();
    auto const  cnt1= bs1.count();

    for( size_t b_c= 0; b_c < 2 * N; ++b_c )
    {
      auto const b_c_mod= b_c % N;
      auto const bs2_r= Bitset2::rotate_right( bs1,   b_c );
      auto const bs2_l= Bitset2::rotate_left(  bs1,   b_c );
      auto const bs2a=  Bitset2::rotate_left(  bs2_r, b_c );
      auto const bs2b=  Bitset2::rotate_right( bs2_l, b_c );
      auto const bs2_r2= ( bs1 >> b_c_mod ) | ( bs1 << (N-b_c_mod) );
      auto const bs2_l2= ( bs1 << b_c_mod ) | ( bs1 >> (N-b_c_mod) );
      if( verbose )
        std::cout << bs1 << "\t"
                  << b_c << "\t"
                  << bs2_l << "\t"
                  << bs2_r << "\n";
      assert( cnt1 == bs2_r.count() );
      assert( cnt1 == bs2_l.count() );
      assert( bs2a == bs1 );
      assert( bs2b == bs1 );
      assert( bs2_r2 == bs2_r );
      assert( bs2_l2 == bs2_l );
    }
  } // for c
} // test_rotate




template<size_t N>
void
test_shift()
{
  std::cout << "Entering test_shift N= " << N << "\n";

  gen_random_bitset2<N>  gen_rand;
  t1<N> const            empty1;

  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1=  gen_rand();

    for( size_t b_c= 0; b_c <= N + 5; ++b_c )
    {
      auto const bs1_l= bs1 << b_c;
      auto const bs1_r= bs1 >> b_c;
      auto       bs1_c1= bs1;
      auto       bs1_c2= bs1;
      bs1_c1 <<= b_c;
      bs1_c2 >>= b_c;
      if( verbose )
        std::cout << bs1 << "\t"
                  << bs1_l << "\t"
                  << bs1_r << "\n";
      if( b_c < N )
      {
        t2<N>  const bs2{ bs1 };
        auto const bs2_l= bs2 << b_c;
        auto const bs2_r= bs2 >> b_c;
        assert( bs2_l == t2<N>( bs1_l ) );
        assert( bs2_r == t2<N>( bs1_r ) );
        assert( bs1_c1 == bs1_l );
        assert( bs1_c2 == bs1_r );
      }
      else
      {
        assert( bs1_l == empty1 );
        assert( bs1_r == empty1 );
        assert( bs1_c1 == empty1 );
        assert( bs1_c2 == empty1 );
      }
    } // for b_c
  } // for c
} // test_shift




template<size_t N>
void
test_add()
{
  std::cout << "Entering test_add N= " << N << "\n";

  gen_random_bitset2<N>  gen_rand;
  dummy_add<N>           adder;
  t1<N>  const           one{{ 1ull }};
  t1<N>  const           all= t1<N>().set();

  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1=  gen_rand();
    auto const  bs2=  gen_rand();
    auto        bs3=  bs1;
    auto        bs4=  bs1;
    auto        bs5=  bs1;
    ++bs3; --bs4;
    bs5 += bs2;

    auto const  add1= bs1 + bs2;
    auto const  add2= adder.add( bs1.data(), bs2.data() );
    auto const  add3= adder.add( bs1.data(), one.data() );
    auto const  add4= adder.add( bs1.data(), all.data() );
    if( verbose )
      std::cout << "  " << bs1 << " + " << bs2
                << "\n= " << add1 << "\n";
    auto const  cmp1= adder.compare( add2, add1.data() );
    auto const  cmp2= adder.compare( add3, bs3.data() );
    auto const  cmp3= adder.compare( add4, bs4.data() );
    auto const  cmp4= adder.compare( add2, bs5.data() );
    assert( cmp1 );
    assert( cmp2 );
    assert( cmp3 );
    assert( cmp4 );
  } // for c
} // test_add




template<size_t N>
void
test_difference()
{
  std::cout << "Entering test_difference N= " << N << "\n";

  gen_random_bitset2<N>  gen_rand;

  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1=  gen_rand();
    auto const  bs2=  gen_rand();
    auto        bs3=  bs1;

    auto const  d1=   Bitset2::difference( bs1, bs2 );
    auto const  d2=   bs1 & ~bs2;
    bs3.difference( bs2 );

    assert( d2  == d1 );
    assert( bs3 == d1 );
  } // for c
} // test_difference





template<size_t N>
void
test_not()
{
  std::cout << "Entering test_not N= " << N << "\n";

  gen_random_bitset2<N>  gen_rand;

  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1=  gen_rand();
    auto const  bs2=  ~bs1;
    for( size_t b_c= 0; b_c < N; ++b_c )
    {
      if( verbose ) std::cout << "~" << bs1 << "\n=" << bs2 << "\n";
      assert( bs1[b_c] != bs2[b_c] );
    }
  } // for c
} // test_not


template<size_t N>
std::vector<size_t>
idx_lst( t1<N> const &bs )
{
  std::vector<size_t> ret_val;
  for( size_t c= 0; c < N; ++c )
    if( bs[c] ) ret_val.push_back( c );
  return ret_val;
} // idx_lst



template<size_t N>
void
test_find()
{
  std::cout << "Entering test_find N= " << N << "\n";

  gen_random_bitset2<N>  gen_rand;

  for( size_t c= 0; c < N; ++ c)
  {
    auto bs1= t1<N>();

    assert( bs1.find_first() == Bitset2::bitset2<N>::npos );
    assert( bs1.find_next(0) == Bitset2::bitset2<N>::npos );

    bs1[c]= true;
    assert( bs1.find_first() == c );
    if( c > 0 )
    {
      assert( bs1.find_next( c - 1 ) == c );

      bs1[0]= true;
      bs1[N-1]= true;
      assert( bs1.find_first() == 0 );
      auto idx= bs1.find_next( c );
      if( c < N - 1 ) assert( idx == N - 1 );
      else            assert( idx == Bitset2::bitset2<N>::npos );

      for( size_t b= 0; b < c; ++b ) bs1[b]= true;
      idx= bs1.find_next( c );
      if( c < N - 1 ) assert( idx == N - 1 );
      else            assert( idx == Bitset2::bitset2<N>::npos );
    }
  } // for c

  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1=  gen_rand();
    auto const  lst=  idx_lst( bs1 );
    if( lst.empty() ) assert( bs1.find_first() == Bitset2::bitset2<N>::npos );
    else
    {
      auto b_it= lst.begin();
      auto e_it= lst.end();
      auto idx=  bs1.find_first();
      assert( idx == *(b_it++) );
      for( ; b_it != e_it; ++b_it )
      {
        idx= bs1.find_next( idx );
        assert( idx == *b_it );
      }
      idx= bs1.find_next( idx );
      assert( idx == Bitset2::bitset2<N>::npos );
    }
  } // for c

} // test_find




template<size_t N>
void
test_bitwise_ops()
{
  std::cout << "Entering test_bitwise_ops N= " << N << "\n";

  gen_random_bitset2<N>  gen_rand;

  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1=   gen_rand();
    auto const  bs2=   gen_rand();
    auto const  b_or=  bs1 | bs2;
    auto const  b_and= bs1 & bs2;
    auto const  b_xor= bs1 ^ bs2;

    auto const  sbs1=   t2<N>( bs1 );
    auto const  sbs2=   t2<N>( bs2 );
    auto const  sb_or=  sbs1 | sbs2;
    auto const  sb_and= sbs1 & sbs2;
    auto const  sb_xor= sbs1 ^ sbs2;

    if( verbose )
      std::cout << "        " << bs1 << " op " << bs2
                << "\nop= |:  " << b_or
                << "\nop= &:  " << b_and
                << "\nop= ^:  " << b_xor << "\n";

    assert( sb_or  == t2<N>( b_or ) );
    assert( sb_and == t2<N>( b_and ) );
    assert( sb_xor == t2<N>( b_xor ) );
  } // for c
} // test_bitwise_ops



int
main()
{
  TESTMANY(test_any_all_none)
  TESTMANY(test_set_count_size)
  TESTMANY(test_set)
  TESTMANY(test_rotate)
  TESTMANY(test_shift)
  TESTMANY(test_add)
  TESTMANY(test_not)
  TESTMANY(test_bitwise_ops)
  TESTMANY(test_difference)
  TESTMANY(test_find)
} // main

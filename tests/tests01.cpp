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
#include <bitset>
#include <cassert>
#include <cstdint>
#include <iostream>

#define TESTMANY(F) \
    F <63 >(); \
    F <64 >(); \
    F <65 >(); \
    F <95 >(); \
    F <96 >(); \
    F <97 >(); \
    F <127>(); \
    F <128>(); \
    F <129>(); \
    F <255>(); \
    F <256>(); \
    F <257>();

#define TESTMANY2(F,T,S) \
    F <63, T>(S); \
    F <64, T>(S); \
    F <65, T>(S); \
    F <95, T>(S); \
    F <96, T>(S); \
    F <97, T>(S); \
    F <127,T>(S); \
    F <128,T>(S); \
    F <129,T>(S); \
    F <255,T>(S); \
    F <256,T>(S); \
    F <257,T>(S);


#define TESTMNY(F) \
    TESTMANY2(F,uint8_t, "uint8_t" ) \
    TESTMANY2(F,uint16_t,"uint16_t") \
    TESTMANY2(F,uint32_t,"uint32_t") \
    TESTMANY2(F,unsigned long long,"U_L_LONG")


template<size_t N,class T=unsigned long long>
using t1= Bitset2::bitset2<N,T>;

template<size_t N>
using t1a= Bitset2::bitset2<N>;

template<size_t N>
using t2= std::bitset<N>;


constexpr size_t n_loops= 100000;

constexpr bool verbose= false;


template<size_t N,class T>
struct dummy_add
{
  enum : size_t
  {   ull_bits= sizeof(T) * CHAR_BIT
    , div_val=  N / ull_bits
    , mod_val=  N % ull_bits
    , n_ull=    ( mod_val != 0 ) ? (div_val+1) : div_val
    , n_array=  ( N == 0 )       ? 1 : n_ull
  };
  //
  enum : T
  {   all_one= T(~T(0))
  ,   hgh_pattern=  (N==0) ? T(0)
                           : (mod_val==0) ? T(all_one)
                                          : T(all_one >> (ull_bits-mod_val))
  };
  //
  using array_t= std::array<T,n_array>;
  //
  array_t
  add( array_t const &a1, array_t const &a2 ) const
  {
    array_t  ret_val;
    T        crry= T(0);

    for( size_t c= 0; c < n_ull; ++c )
    {
      T const v= T( a1[c] + a2[c] + crry );
      if( v < a1[c] || v < a2[c] ||
          ( a1[c] == all_one && a2[c] == all_one ) ) crry= T(1);
      else                                           crry= T(0);
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


template<size_t N,class T>
t1<N,T>
dummy_reverse( t1<N,T> const & bs )
{
  t1<N,T>  ret_val;
  for( size_t c= 0; c < N; ++c ) ret_val[c]= bs[N-c-1];

  return ret_val;
}



template<size_t N,class T>
void
test_any_all_none( char const * type_str )
{
  std::cout << "Entering test_any_all_none N= " << N << " type= " << type_str << "\n";

  t1<N,T> const empty1;
  t2<N>   const empty2;
  auto  const   full1= ~empty1;
  auto  const   full2= ~empty2;

  auto const  empty1a= t1<N,T>( empty2 );
  auto const  empty2a= t2<N>(   empty1 );
  auto const  full1a=  t1<N,T>( full2 );
  auto const  full2a=  t2<N>(   full1 );

  assert(  empty1.none()  && !empty1.all()  && !empty1.any() );
  assert(  empty1a.none() && !empty1a.all() && !empty1a.any() );
  assert(  empty2a.none() && !empty2a.all() && !empty2a.any() );
  assert( !full1.none()   &&  full1.all()   &&  full1.any() );
  assert( !full1a.none()  &&  full1a.all()  &&  full1a.any() );
  assert( !full2a.none()  &&  full2a.all()  &&  full2a.any() );


  constexpr t1<N,T> ce_empty1;
  constexpr auto    ce_full1= ~ce_empty1;
  static_assert(  ce_empty1.none()  && !ce_empty1.all()  && !ce_empty1.any(), "" );
  static_assert( !ce_full1.none()   &&  ce_full1.all()   &&  ce_full1.any(), "" );


  gen_random_bitset2<N,T>  gen_rand;
  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1= gen_rand();
    auto const  bs2=  t2<N>( bs1 );
    auto const  bs1a= t1<N,T>( bs2 );

    if( verbose ) std::cout << bs1.to_hex_string() << "\t" << c << "\n";

    assert( bs1 == bs1a );
    assert( bs1.none() == bs2.none() );
    assert( bs1.all()  == bs2.all() );
    assert( bs1.any()  == bs2.any() );
  } // for c
} // test_any_all_none




template<size_t N,class T>
void
test_set_count_size( char const * type_str )
{
  std::cout << "Entering test_set_count_size N= " << N << " type= " << type_str << "\n";

  t1<N,T> const      empty1;
  constexpr t1<N,T>  ce_empty1;
  constexpr t1<N,T>  ce_full1= ~ce_empty1;
  assert( empty1.size() == N );
  static_assert( ce_empty1.size() == N, "" );
  static_assert( ce_full1.count() == N, "" );
  static_assert( !ce_empty1.test( N - 3 ), "" );
  static_assert( ce_full1.test( N - 2 ), "" );

  gen_random_bitset2<N,T>  gen_rand;
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




template<size_t N,class T>
void
test_set( char const * type_str )
{
  std::cout << "Entering test_set N= " << N << " type= " << type_str << "\n";

  gen_random_bitset2<N,T>  gen_rand;
  for( size_t c= 0; c < n_loops; ++c )
  {
    auto  bs1=  gen_rand();
    auto  bs2=  bs1;
    auto  bs3=  t2<N>( bs1 );
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
    assert( bs1 == (t1<N,T>( bs3 )) );
  } // for c
} // test_set




template<size_t N,class T>
void
test_rotate( char const * type_str )
{
  std::cout << "Entering test_rotate N= " << N << " type= " << type_str << "\n";

  constexpr std::array<T,2>  ce_arr1{{ T(2), T(5) }};
  constexpr std::array<T,2>  ce_arr2{{ T(4), T(10) }};
  constexpr t1<74,T>  ce_bs1( ce_arr1 );
  constexpr t1<74,T>  ce_bs1_r= Bitset2::rotate_left( ce_bs1, 1 );
  constexpr t1<74,T>  ce_bs2( ce_arr2 );
  static_assert( ce_bs1_r == ce_bs2, "" );

  gen_random_bitset2<N,T>  gen_rand;
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

      auto bc1_c1= bs1;
      auto bc1_c2= bs1;
      bc1_c1.rotate_left(  b_c );
      bc1_c2.rotate_right( b_c );

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
      assert( bc1_c1 == bs2_l );
      assert( bc1_c2 == bs2_r );
    }
  } // for c
} // test_rotate




template<size_t N,class T>
void
test_shift( char const * type_str )
{
  std::cout << "Entering test_shift N= " << N << " type= " << type_str << "\n";

  gen_random_bitset2<N,T>  gen_rand;
  t1<N,T> const            empty1;

  constexpr size_t n_bts_m= sizeof(T) * CHAR_BIT - 1;
  constexpr std::array<T,2>  ce_arr1{{ T(2), T(5) }};
  constexpr std::array<T,2>  ce_arr2{{ T(4), T(10) }};
  constexpr std::array<T,2>  ce_arr3{{ T(1) + T(T(1) << n_bts_m), T(2) }};
  constexpr t1<74,T>  ce_bs1( ce_arr1 );
  constexpr t1<74,T>  ce_bs1_s1= ce_bs1 << 1;
  constexpr t1<74,T>  ce_bs1_s2= ce_bs1 >> 1;
  constexpr t1<74,T>  ce_bs2( ce_arr2 );
  constexpr t1<74,T>  ce_bs3( ce_arr3 );
  static_assert( ce_bs1_s1 == ce_bs2, "" );
  static_assert( ce_bs1_s2 == ce_bs3, "" );

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




template<size_t N,class T>
void
test_add( const char * type_str )
{
  std::cout << "Entering test_add N= " << N << " type= " << type_str << "\n";

  gen_random_bitset2<N,T>  gen_rand;
  dummy_add<N,T>           adder;
  t1<N,T> const            one{{ T(1) }};
  t1<N,T> const            all= t1<N,T>().set();

  auto const               all_twice=  all + all;
  auto                     all_twice2= all;
  all_twice2 += all;
  auto                     a_m1= all;
  a_m1--;
  auto                     a_m2= a_m1;
  a_m2--;
  auto                     all_twice_m1= all + a_m1;
  assert( all_twice    == a_m1 );
  assert( all_twice2   == a_m1 );
  assert( all_twice_m1 == a_m2 );

  constexpr t1<N,T>        ce_one{{ T(1) }};
  constexpr t1<N,T>        ce_empty;
  constexpr t1<N,T>        ce_all= ~ce_empty;
  constexpr t1<N,T>        ce_all_but_one= ~ce_one;
  constexpr auto           ce_all_twice= ce_all + ce_all;
  static_assert( ce_all_twice == ce_all_but_one, "" );

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
      std::cout << "  " << bs1 << "\n+ " << bs2
                << "\n= " << add1
                << "\n, " << t1<N,T>(add2) << "\n\n";
    auto const  cmp1= adder.compare( add2, add1.data() );
    auto const  cmp2= adder.compare( add3, bs3.data() );
    auto const  cmp3= adder.compare( add4, bs4.data() );
    auto const  cmp4= adder.compare( add2, bs5.data() );
    if( !cmp3 )
      std::cout << "  " << bs1  << '\n'
                << "+ " << all  << "    --" << bs1
                << "\n= " << t1<N,T>(add4) << "    = " << bs4 << "\n";
    assert( cmp1 );
    assert( cmp2 );
    assert( cmp3 );
    assert( cmp4 );
  } // for c
} // test_add




template<size_t N,class T>
void
test_difference( char const * type_str )
{
  std::cout << "Entering test_difference N= " << N << " type= " << type_str << "\n";

  constexpr std::array<T,2>  ce_arr1{{ T(3), T(5) }};
  constexpr std::array<T,2>  ce_arr2{{ T(6), T(1) }};
  constexpr t1<74,T>  ce_bs1( ce_arr1 );
  constexpr t1<74,T>  ce_bs2( ce_arr2 );
  constexpr auto      ce_diff1= Bitset2::difference( ce_bs1, ce_bs2 );
  constexpr auto      ce_ref1=  ce_bs1 & ~ce_bs2;
  static_assert( ce_diff1 == ce_ref1, "" );

  gen_random_bitset2<N,T>  gen_rand;

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





template<size_t N,class T>
void
test_not( char const * type_str )
{
  std::cout << "Entering test_not N= " << N << " type= " << type_str << "\n";

  gen_random_bitset2<N,T>  gen_rand;

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


template<size_t N,class T>
std::vector<size_t>
idx_lst( t1<N,T> const &bs )
{
  std::vector<size_t> ret_val;
  for( size_t c= 0; c < N; ++c )
    if( bs[c] ) ret_val.push_back( c );
  return ret_val;
} // idx_lst



template<size_t N,class T>
void
test_find( char const * type_str )
{
  std::cout << "Entering test_find N= " << N << " type= " << type_str << "\n";

  constexpr  t1<N,T>  ce_bs1( 12ull );
  static_assert( ce_bs1.find_first() == 2, "" );
  static_assert( ce_bs1.find_next( 2 ) == 3, "" );
  static_assert( ce_bs1.find_next( 3 ) == t1<N,T>::npos, "" );

  gen_random_bitset2<N,T>  gen_rand;

  for( size_t c= 0; c < N; ++ c)
  {
    auto bs1= t1<N,T>();

    assert( bs1.find_first() == (Bitset2::bitset2<N,T>::npos) );
    assert( bs1.find_next(0) == (Bitset2::bitset2<N,T>::npos) );

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
      else            assert( idx == (Bitset2::bitset2<N,T>::npos) );

      for( size_t b= 0; b < c; ++b ) bs1[b]= true;
      idx= bs1.find_next( c );
      if( c < N - 1 ) assert( idx == N - 1 );
      else            assert( idx == (Bitset2::bitset2<N,T>::npos) );
    }
  } // for c

  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1=  gen_rand();
    auto const  lst=  idx_lst( bs1 );
    if( lst.empty() ) assert( bs1.find_first() == (Bitset2::bitset2<N,T>::npos) );
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
      assert( idx == (Bitset2::bitset2<N,T>::npos) );
    }
  } // for c

} // test_find




template<size_t N,class T>
void
test_bitwise_ops( char const * type_str )
{
  std::cout << "Entering test_bitwise_ops N= " << N << " type= " << type_str << "\n";

  constexpr std::array<T,2>  ce_arr1{{ T(3), T(5) }};
  constexpr std::array<T,2>  ce_arr2{{ T(5), T(1) }};
  constexpr std::array<T,2>  ce_e_or{{  T(7), T(5) }};
  constexpr std::array<T,2>  ce_e_and{{ T(1), T(1) }};
  constexpr std::array<T,2>  ce_e_xor{{ T(6), T(4) }};
  constexpr t1<74,T>  ce_bs1( ce_arr1 );
  constexpr t1<74,T>  ce_bs2( ce_arr2 );
  constexpr t1<74,T>  ce_bs_e_or(  ce_e_or );
  constexpr t1<74,T>  ce_bs_e_and( ce_e_and );
  constexpr t1<74,T>  ce_bs_e_xor( ce_e_xor );
  constexpr auto      ce_or=  ce_bs1 | ce_bs2;
  constexpr auto      ce_and= ce_bs1 & ce_bs2;
  constexpr auto      ce_xor= ce_bs1 ^ ce_bs2;
  static_assert( ce_or  == ce_bs_e_or,  "" );
  static_assert( ce_and == ce_bs_e_and, "" );
  static_assert( ce_xor == ce_bs_e_xor, "" );

  gen_random_bitset2<N,T>  gen_rand;

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




template<size_t N,class T>
void
test_reverse( char const * type_str )
{
  std::cout << "Entering test_reverse N= " << N << " type= " << type_str << "\n";

  constexpr t1<N,T>  ce_bs1( 3ull );
  constexpr auto     ce_bs1_rev= Bitset2::reverse( ce_bs1 );
  constexpr auto     ce_bs1_rot= Bitset2::rotate_right( ce_bs1, 2 );
  static_assert( ce_bs1_rev == ce_bs1_rot, "" );

  gen_random_bitset2<N,T>  gen_rand;

  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1=   gen_rand();
    auto        bs2=   bs1;
    auto const  bs3=   dummy_reverse( bs1 );
    auto const  bs4=   Bitset2::reverse( bs1 );
    bs2.reverse();
    if( verbose )
      std::cout << "  " << bs1
                << "\n2 " << bs2
                << "\n3 " << bs3 << '\n';
    assert( bs2 == bs3 );
    assert( bs4 == bs3 );
    bs2.reverse();
    assert( bs2 == bs1 );
  } // for c
} // test_reverse



template<size_t N,class T>
void
test_convert( char const * type_str )
{
  std::cout << "Entering test_convert N= " << N << " type= " << type_str << "\n";

  constexpr t1<N,T>  ce_bs1( 0ull );
  constexpr auto     ce_bs2= ~ce_bs1;
  constexpr auto     ce_bs2a= ce_bs2 >> 1;
  constexpr auto     ce_bs2b= Bitset2::convert_to<N-1>( ce_bs2 );
  constexpr auto     ce_bs2c= Bitset2::convert_to<N>(   ce_bs2b );
  constexpr auto     ce_bs2d= Bitset2::convert_to<N+1>( ce_bs2 );
  constexpr auto     ce_bs2e= Bitset2::convert_to<N>(   ce_bs2d );
  static_assert( ce_bs2c == ce_bs2a, "" );
  static_assert( ce_bs2e == ce_bs2,  "" );

  gen_random_bitset2<N,T>  gen_rand;

  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1=   gen_rand();
    auto const  bs2a= bs1 & ce_bs2a;
    auto const  bs2b= Bitset2::convert_to<N-1>( bs1 );
    auto const  bs2c= Bitset2::convert_to<N>(   bs2b );
    auto const  bs2d= Bitset2::convert_to<N+1>( bs1 );
    auto const  bs2e= Bitset2::convert_to<N>(   bs2d );
    assert( bs2c == bs2a );
    assert( bs2e == bs1 );
  } // for c
} // test_convert




template<size_t N,class T>
void
test_compare( char const * type_str )
{
  std::cout << "Entering test_compare N= " << N << " type= " << type_str << "\n";

  constexpr t1<N,T>  ce_bs1( 0ull );
  constexpr auto     ce_bs2= ~ce_bs1;
  constexpr auto     ce_bs2a= ce_bs2 >> 1;
  constexpr auto     ce_bs2b= ce_bs2a;
  static_assert( ce_bs2a <  ce_bs2,  "" );
  static_assert( ce_bs2b <= ce_bs2a, "" );
  static_assert( ce_bs2  >  ce_bs2a,  "" );
  static_assert( ce_bs2a >= ce_bs2b, "" );
  static_assert( ce_bs2a != ce_bs2,  "" );

  gen_random_bitset2<N,T>  gen_rand;

  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1=   gen_rand();
    if( bs1 != ce_bs2 )
    {
      auto bs2= bs1;
      auto bs3= bs1;
      --bs2;
      assert( bs2 <  bs1 );
      assert( bs2 <= bs1 );
      assert( bs1 >  bs2 );
      assert( bs1 >= bs1 );
      assert( bs3 <= bs1 );
      assert( bs3 >= bs1 );
    }
    else
    {
      assert( bs1 == ce_bs2 );
    }
  } // for c
} // test_compare



template<size_t N>
void
test_hash()
{
  std::cout << "Entering test_hash N= " << N << "\n";

  gen_random_bitset2<N,uint8_t>   gen_rand8;
  gen_random_bitset2<N,uint16_t>  gen_rand16;
  gen_random_bitset2<N,uint32_t>  gen_rand32;
  gen_random_bitset2<N,uint64_t>  gen_rand64;

  for( size_t c= 0; c < n_loops; ++c )
  {
    auto const  bs1_8=   gen_rand8();
    auto const  bs1a_8=  t2<N>( bs1_8 );
    auto const  bs2_8=   t1<N,uint16_t>( bs1a_8 );
    auto const  bs3_8=   t1<N,uint32_t>( bs1a_8 );
    auto const  bs4_8=   t1<N,uint64_t>( bs1a_8 );

    auto const hs1_8=  std::hash<std::remove_cv_t<decltype(bs1_8)> >{}( bs1_8 );
    auto const hs2_8=  std::hash<std::remove_cv_t<decltype(bs2_8)> >{}( bs2_8 );
    auto const hs3_8=  std::hash<std::remove_cv_t<decltype(bs3_8)> >{}( bs3_8 );
    auto const hs4_8=  std::hash<std::remove_cv_t<decltype(bs4_8)> >{}( bs4_8 );
    assert( hs1_8 == hs2_8 );
    assert( hs1_8 == hs3_8 );
    assert( hs1_8 == hs4_8 );


    auto const  bs1_16=   gen_rand16();
    auto const  bs1a_16=  t2<N>( bs1_16 );
    auto const  bs2_16=   t1<N,uint8_t>(  bs1a_16 );
    auto const  bs3_16=   t1<N,uint32_t>( bs1a_16 );
    auto const  bs4_16=   t1<N,uint64_t>( bs1a_16 );

    auto const hs1_16=  std::hash<std::remove_cv_t<decltype(bs1_16)> >{}( bs1_16 );
    auto const hs2_16=  std::hash<std::remove_cv_t<decltype(bs2_16)> >{}( bs2_16 );
    auto const hs3_16=  std::hash<std::remove_cv_t<decltype(bs3_16)> >{}( bs3_16 );
    auto const hs4_16=  std::hash<std::remove_cv_t<decltype(bs4_16)> >{}( bs4_16 );
    assert( hs1_16 == hs2_16 );
    assert( hs1_16 == hs3_16 );
    assert( hs1_16 == hs4_16 );


    auto const  bs1_32=   gen_rand32();
    auto const  bs1a_32=  t2<N>( bs1_32 );
    auto const  bs2_32=   t1<N,uint8_t>(  bs1a_32 );
    auto const  bs3_32=   t1<N,uint16_t>( bs1a_32 );
    auto const  bs4_32=   t1<N,uint64_t>( bs1a_32 );

    auto const hs1_32=  std::hash<std::remove_cv_t<decltype(bs1_32)> >{}( bs1_32 );
    auto const hs2_32=  std::hash<std::remove_cv_t<decltype(bs2_32)> >{}( bs2_32 );
    auto const hs3_32=  std::hash<std::remove_cv_t<decltype(bs3_32)> >{}( bs3_32 );
    auto const hs4_32=  std::hash<std::remove_cv_t<decltype(bs4_32)> >{}( bs4_32 );
    assert( hs1_32 == hs2_32 );
    assert( hs1_32 == hs3_32 );
    assert( hs1_32 == hs4_32 );


    auto const  bs1_64=   gen_rand64();
    auto const  bs1a_64=  t2<N>( bs1_64 );
    auto const  bs2_64=   t1<N,uint8_t>(  bs1a_64 );
    auto const  bs3_64=   t1<N,uint16_t>( bs1a_64 );
    auto const  bs4_64=   t1<N,uint32_t>( bs1a_64 );

    auto const hs1_64=  std::hash<std::remove_cv_t<decltype(bs1_64)> >{}( bs1_64 );
    auto const hs2_64=  std::hash<std::remove_cv_t<decltype(bs2_64)> >{}( bs2_64 );
    auto const hs3_64=  std::hash<std::remove_cv_t<decltype(bs3_64)> >{}( bs3_64 );
    auto const hs4_64=  std::hash<std::remove_cv_t<decltype(bs4_64)> >{}( bs4_64 );
    assert( hs1_64 == hs2_64 );
    assert( hs1_64 == hs3_64 );
    assert( hs1_64 == hs4_64 );
  } // for c
} // test_hash



int
main()
{
  std::cout << "sizeof( bitset2<8>  )= " << sizeof( t1a<8>  ) << '\n';
  std::cout << "sizeof( bitset2<16> )= " << sizeof( t1a<16> ) << '\n';
  std::cout << "sizeof( bitset2<32> )= " << sizeof( t1a<32> ) << '\n';
  std::cout << "sizeof( bitset2<64> )= " << sizeof( t1a<64> ) << '\n';
  std::cout << "sizeof( bitset2<65> )= " << sizeof( t1a<65> ) << '\n';

  TESTMANY(test_hash)

  TESTMNY(test_compare)
  TESTMNY(test_convert)
  TESTMNY(test_add)
  TESTMNY(test_reverse)
  TESTMNY(test_find)
  TESTMNY(test_difference)
  TESTMNY(test_any_all_none)
  TESTMNY(test_set_count_size)
  TESTMNY(test_set)
  TESTMNY(test_not)
  TESTMNY(test_bitwise_ops)
  TESTMNY(test_shift)
  TESTMNY(test_rotate)

} // main

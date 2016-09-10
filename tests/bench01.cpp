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
#include <cassert>
#include <chrono>



using ULLONG= unsigned long long;

template<size_t N>
using t1= Bitset2::bitset2<N>;

template<size_t N>
using vec_t= std::vector<t1<N> >;


constexpr size_t n_loops= 1000000;

template<size_t N>
vec_t<N>
gen_bs_vec( size_t n )
{
  std::vector<t1<N> >    ret_val;
  gen_random_bitset2<N>  gen_rand;
  ret_val.reserve( n );
  for( size_t c= 0; c < n; ++c ) ret_val.push_back( gen_rand() );

  return ret_val;
} // gen_bs_vec


template<size_t N>
void
apply_or_equal( vec_t<N> & v1, vec_t<N> const & v2 )
{
  auto it2= v2.begin();
  for( auto & bs: v1 ) bs |= *(it2++);
} // apply_or_equal


template<size_t n>
std::array<ULLONG,n> &
rm_const( std::array<ULLONG,n> const & a )
{ return const_cast<std::array<ULLONG,n> &>(a); }


template<size_t N>
void
array_or_equal( vec_t<N> & v1, vec_t<N> const & v2 )
{
  auto const n_array= v1[0].data().size();
  auto it2= v2.begin();
  for( auto & bs: v1 )
  {
    auto & a1= rm_const( bs.data() );
    auto & a2= it2->data();
    for( size_t c= 0; c < n_array; ++c ) a1[c] |= a2[c];

    ++it2;
  }
}


int main()
{
  auto const vec1= gen_bs_vec<2048>( 128 );
  auto const vec2= gen_bs_vec<2048>( 128 );

  std::cout << "Running computations\n";

  auto const t1 = std::chrono::high_resolution_clock::now();

  auto v1= vec1;
  for( size_t c= 0; c < n_loops; ++c )
  {
    v1= vec1;
    apply_or_equal( v1, vec2 );
  }

  auto const t2 = std::chrono::high_resolution_clock::now();

  auto v1a= vec1;
  for( size_t c= 0; c < n_loops; ++c )
  {
    v1a= vec1;
    array_or_equal( v1a, vec2 );
  }

  auto const t3 = std::chrono::high_resolution_clock::now();

  assert( v1 == v1a );

  for( size_t c= 0; c < n_loops; ++c )
  {
    v1a= vec1;
    array_or_equal( v1a, vec2 );
  }

  auto const t4 = std::chrono::high_resolution_clock::now();

  for( size_t c= 0; c < n_loops; ++c )
  {
    v1= vec1;
    apply_or_equal( v1, vec2 );
  }

  auto const t5 = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> dt21 = t2 -t1;
  const std::chrono::duration<double> dt32 = t3 -t2;
  const std::chrono::duration<double> dt43 = t4 -t3;
  const std::chrono::duration<double> dt54 = t5 -t4;

  std::cout << "dt21= " << dt21.count() * 1.0e3 << "ms\n";
  std::cout << "dt32= " << dt32.count() * 1.0e3 << "ms\n";
  std::cout << "dt43= " << dt43.count() * 1.0e3 << "ms\n";
  std::cout << "dt54= " << dt54.count() * 1.0e3 << "ms\n";
} // main

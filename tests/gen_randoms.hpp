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


#ifndef BITSET2_GEN_RANDOMS_CB_HPP
#define BITSET2_GEN_RANDOMS_CB_HPP

#include <random>
#include <climits>
#include <array>
#include <vector>



class gen_randoms
{
public:
  gen_randoms( unsigned long long max_val, unsigned long long min_val= 0 )
  : m_generator( std::random_device{}() )
  , m_distri( min_val, max_val )
  {}
  //
  unsigned long long
  operator()()
  { return m_distri( m_generator ); }
  //
private:
  std::mt19937                                        m_generator;
  std::uniform_int_distribution<unsigned long long>   m_distri;
}; // class gen_randoms




/// Generate N-bit sized bitset2 randomly
template<size_t N>
class gen_random_bitset2
{
  using ULLONG= unsigned long long;
  //
  enum : size_t 
  {   ullong_bits=  sizeof(ULLONG) * CHAR_BIT 
    , div_val=      N / ullong_bits
    , mod_val=      N % ullong_bits
    , n_ullong=     mod_val ? div_val + 1 : div_val
  };
  enum : ULLONG
  {   max_ullong= (~(0ull))
    , hgh_bit_pattern=
              N == 0 ? 0
              : mod_val == 0
                ? max_ullong
                : max_ullong >> (ullong_bits-mod_val)
  };
  //
  using arr_t=  std::array<ULLONG,n_ullong>;
  //
public:
  using b_t= Bitset2::bitset2<N>;
  //
  gen_random_bitset2()
  : m_gen{ gen_generators() }
  {}
  //
  b_t
  operator()()
  {
    arr_t  a;
    for( size_t c= 0; c < n_ullong; ++c ) a[c]= m_gen[c]();
    return b_t( a );
  }
  //
private:
  std::vector<gen_randoms>  m_gen;
  //
  std::vector<gen_randoms>
  gen_generators()
  {
    std::vector<gen_randoms>  ret_val;
    
    if( n_ullong > 0 )
    {
      ret_val.reserve( n_ullong );
      for( size_t c= 0; c < n_ullong - 1; ++c ) ret_val.push_back( gen_randoms{ max_ullong } );
      ret_val.push_back( gen_randoms{ mod_val > 0 ? hgh_bit_pattern : max_ullong } );
    }
    
    return ret_val;
  }
}; // gen_random_bitset2




#endif // BITSET2_GEN_RANDOMS_CB_HPP

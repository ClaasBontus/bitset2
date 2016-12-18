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



template<class T>
class gen_randoms
{
public:
  gen_randoms( T max_val, T min_val= 0 )
  : m_generator( std::random_device{}() )
  , m_distri( min_val, max_val )
  {}
  //
  T
  operator()()
  { return m_distri( m_generator ); }
  //
private:
  std::mt19937                                        m_generator;
  std::uniform_int_distribution<T>                    m_distri;
}; // class gen_randoms




/// Generate N-bit sized bitset2 randomly
template<size_t N,class T>
class gen_random_bitset2
{
  //
  enum : size_t
  {   base_t_n_bits=  sizeof(T) * CHAR_BIT
    , div_val=        N / base_t_n_bits
    , mod_val=        N % base_t_n_bits
    , n_words=        mod_val ? div_val + 1 : div_val
  };
  enum : T
  {   max_ullong= T(~T(0))
    , hgh_bit_pattern=
              N == 0 ? T(0)
              : mod_val == 0
                ? max_ullong
                : max_ullong >> T(base_t_n_bits-mod_val)
  };
  //
  using arr_t=  std::array<T,n_words>;
  //
public:
  using b_t= Bitset2::bitset2<N,T>;
  //
  gen_random_bitset2()
  : m_gen{ gen_generators() }
  {}
  //
  b_t
  operator()()
  {
    arr_t  a;
    for( size_t c= 0; c < n_words; ++c ) a[c]= m_gen[c]();
    return b_t( a );
  }
  //
private:
  using gen_rand_t= gen_randoms<T>;

  std::vector<gen_rand_t>  m_gen;
  //
  std::vector<gen_rand_t>
  gen_generators()
  {
    std::vector<gen_rand_t>  ret_val;

    if( n_words > 0 )
    {
      ret_val.reserve( n_words );
      for( size_t c= 0; c < n_words - 1; ++c )
          ret_val.push_back( gen_rand_t{ max_ullong } );
      ret_val.push_back( gen_rand_t{ hgh_bit_pattern } );
    }

    return ret_val;
  }
}; // gen_random_bitset2




#endif // BITSET2_GEN_RANDOMS_CB_HPP

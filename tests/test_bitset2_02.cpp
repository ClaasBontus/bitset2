
#include <iostream>
#include "bitset2.hpp"


int main()
{
  using namespace Bitset2;

  using ULLONG= unsigned long long;

  constexpr
  std::array<ULLONG,1>  a01{{1ull}};

  constexpr
  std::array<ULLONG,0>  a02{};

  constexpr auto a_app01= detail::array_funcs<1>().prepend( 2ull, a01 );
  constexpr auto a_app02= detail::array_funcs<0>().prepend( 3ull, a02 );
  std::cout << a_app01[0] << "\t" << a_app01[1] << "\n";
  std::cout << a_app02[0] << "\n";
  
  //constexpr 
  auto aop0=  detail::array_ops<65>( 0 );
  //constexpr 
  auto aop65= detail::array_ops<65>( 65 );
  //constexpr 
  auto slp65= Bitset2::bitset2<64>{ aop65.m_shft_left_pattern };
  
  std::cout << "(aop0)  m_n_shift_mod= " << aop0.m_n_shift_mod
            << "\n        m_shft_div=    " << aop0.m_shft_div
            << "\n        m_shft_mod=    " << aop0.m_shft_mod
            << "\n        m_shft_leftright_shift= " << aop0.m_shft_leftright_shift
            << "\n        m_shft_left_pattern=    " << aop0.m_shft_left_pattern
            << "\n        m_shft_right_pattern=   " << aop0.m_shft_right_pattern
            << "\n        n_ullong=               " << aop0.n_ullong
            << "\n        n_array=                " << aop0.n_array
            << "\n";
  std::cout << "(aop65) m_n_shift_mod= " << aop65.m_n_shift_mod
            << "\n        m_shft_div=    " << aop65.m_shft_div
            << "\n        m_shft_mod=    " << aop65.m_shft_mod
            << "\n        m_shft_leftright_shift= " << aop65.m_shft_leftright_shift
            << "\n        m_shft_left_pattern=    " << aop65.m_shft_left_pattern
            << "\n                           =    " << slp65
            << "\n        m_shft_right_pattern=   " << aop65.m_shft_right_pattern
            << "\n        n_ullong=               " << aop65.n_ullong
            << "\n        n_array=                " << aop65.n_array
            << "\n";
}

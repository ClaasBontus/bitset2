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

#ifndef BITSET2_HEX_PARAMS_CB_HPP
#define BITSET2_HEX_PARAMS_CB_HPP

#include <string>


namespace Bitset2
{


template< class CharT = char,
          class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT> >
struct hex_params
{
  using str_t= std::basic_string<CharT,Traits,Allocator>;

  CharT        zeroCh=         CharT( '0' );
  CharT        aCh=            CharT( 'a' );
  bool         leadingZeroes=  true;
  bool         nonEmpty=       true;
  str_t        prefix=         str_t{};
}; // struct hex_params



} // namespace Bitset2


#endif // BITSET2_HEX_PARAMS_CB_HPP

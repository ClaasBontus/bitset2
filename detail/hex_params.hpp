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

  hex_params( CharT         zero_ch=        CharT('0'),
              CharT         a_ch=           CharT('a'),
              bool          leading_zeroes= true,
              bool          non_empty=      true,
              str_t const & prfx=           str_t{} )
  : zeroCh{ zero_ch }
  , aCh{ a_ch }
  , leadingZeroes{ leading_zeroes }
  , nonEmpty{ non_empty }
  , prefix{ prfx }
  {}

  CharT        zeroCh=         CharT( '0' );
  CharT        aCh=            CharT( 'a' );
  bool         leadingZeroes=  true;
  bool         nonEmpty=       true;
  str_t        prefix;
}; // struct hex_params



} // namespace Bitset2


#endif // BITSET2_HEX_PARAMS_CB_HPP

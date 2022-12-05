//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace decoder.h: xschie03                  //
//    Datum: 05. 12. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __DECODER_H__
#define __DECODER_H__

#include "./singleton.h"

#define is_octal_digit(c) ((c >= '0') && (c <= '7'))
#define is_hex_digit(c) ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
#define read_hex_digit(c) ((c & 0xf) + (c > 9 ? 9 : 0))

singleton_t* decoder_decode_string(singleton_t* str);

#endif

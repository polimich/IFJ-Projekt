//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace encoder.h: xschie03                  //
//    Datum: 05. 12. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "./singleton.h"

singleton_t* encoder_encode_string(singleton_t* str);

#endif

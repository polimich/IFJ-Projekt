//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace scanner.h: xschie03                  //
//    Datum: 9. 10. 2022                                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <ctype.h>
#include <stdbool.h>

#include "./singleton.h"
#include "./varstring.h"

singleton_t* lexer_get_token(FILE* input);


#endif

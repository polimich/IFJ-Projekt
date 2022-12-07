//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace varstring.c: xschie03                //
//    Datum: 9. 10. 2022                                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./varstring.h"

extern inline varstring_t* varstring_init();

extern inline singleton_t* varstring_read(varstring_t* str);

extern inline void varstring_write(varstring_t* str, const char* fmt, ...);

extern inline singleton_t* varstring_destroy(varstring_t* str);

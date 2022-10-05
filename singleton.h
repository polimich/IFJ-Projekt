//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace singleton.h: xschie03                //
//    Datum: 4. 10. 2022                                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <string.h>

#include "./hash.h"
#include "./memory.h"

typedef struct singleton_t {
    size_t hash;
    struct singleton_t* next;
    const char* strval;
} singleton_t;

singleton_t* find_singleton(singleton_t* singleton, const char* restrict strval);

singleton_t* get_singleton(const char* restrict strval);

#endif

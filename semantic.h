//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace semantic.h: xlukas18                 //
//    Datum: 28. 11. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

#include "./ast.h"
#include "./error.h"
#include "./memory.h"
#include "./symbol.h"
#include "./symtable.h"

typedef enum semantic_type {
    semantic_type_int,
    semantic_type_float,
    semantic_type_string,
    semantic_type_bool,
    semantic_type_dynamic,
} semantic_type_t;

#endif

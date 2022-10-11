//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace symbol.h: xschie03                   //
//    Datum: 11. 10. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "./singleton.h"

typedef enum symbol_type {
    symbol_type_undefined = 1,
    symbol_type_function_identifier,
    symbol_type_global_variable_identifier,
    symbol_type_local_variable_identifier,
    symbol_type_operator,
} symbol_type_t;

typedef struct symbol_t {
    symbol_type_t type;
    singleton_t* value;

    long int line_number;

    // next entry in symtable
    struct symbol_t* next;
} symbol_t;

#endif

//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace symbol.h: xschie03                   //
//    Datum: 11. 10. 2022 - 19. 10. 2022                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <stdbool.h>

#include "./singleton.h"

typedef enum symbol_type {
    symbol_type_undefined = 1,
    symbol_type_keyword,
    symbol_type_operator,
    symbol_type_function_identifier,
    symbol_type_constant,
    symbol_type_global_variable,
    symbol_type_local_variable,
} symbol_type_t;

typedef struct symbol_t {
    symbol_type_t type;
    singleton_t* str;

    struct symbol_t* scope; // OPTIONAL function identifier

    int line_number;

    singleton_t* constant_type;
    int constant_value_int;
    float constant_value_float;

    bool variable_is_declared;
    bool variable_has_value;

    // next entry in global symtable
    struct symbol_t* next;
} symbol_t;

symbol_t* get_symbol_scoped(symbol_type_t type, singleton_t* value, symbol_t* scope);

symbol_t* get_symbol(symbol_type_t type, singleton_t* value);

symbol_t* get_symbol_by_str(symbol_type_t type, const char* str);

#endif

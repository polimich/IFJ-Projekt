//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace symtable.c: xschie03                 //
//    Datum: 11. 10. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./symtable.h"

#define SYMBOL_MAP_SIZE 512

symbol_t* symbols[SYMBOL_MAP_SIZE] = { NULL };

inline size_t get_symbol_index(symbol_type_t type, singleton_t* value)
{
    return ((size_t)type * (size_t)value * (size_t)value) % SYMBOL_MAP_SIZE;
}

symbol_t* create_symbol(symbol_type_t type, singleton_t* value)
{
    symbol_t* symbol = (symbol_t*)mem_alloc_safe(sizeof(*symbol));

    symbol->type = type;
    symbol->str = value;

    size_t index = get_symbol_index(type, value);

    symbol->next = symbols[index];
    symbols[index] = symbol;

    return symbol;
}

symbol_t* find_symbol(symbol_type_t type, singleton_t* value)
{
    for (symbol_t* symbol = symbols[get_symbol_index(type, value)]; symbol != NULL; symbol = symbol->next) {
        if (symbol->type == type && symbol->str == value) {
            return symbol;
        }
    }

    return NULL;
}

symbol_t* get_symbol(symbol_type_t type, singleton_t* value)
{
    symbol_t* found = find_symbol(type, value);

    if (found != NULL) {
        return found;
    }

    return create_symbol(type, value);
}

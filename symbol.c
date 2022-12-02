//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace symbol.c: xschie03                   //
//    Datum: 11. 10. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./symbol.h"

#define SYMBOL_MAP_SIZE 512

symbol_t* symbols[SYMBOL_MAP_SIZE] = { NULL };

extern inline size_t get_symbol_index(symbol_type_t type, singleton_t* value, symbol_t* scope)
{
    return ((size_t)type * (size_t)value * (size_t)value + (size_t)scope) % SYMBOL_MAP_SIZE;
}

symbol_t* create_symbol(symbol_type_t type, singleton_t* value, symbol_t* scope)
{
    salloc(symbol_t, symbol);

    symbol->type = type;
    symbol->str = value;

    size_t index = get_symbol_index(type, value, scope);

    symbol->next = symbols[index];
    symbols[index] = symbol;

    return symbol;
}

symbol_t* find_symbol(symbol_type_t type, singleton_t* value, symbol_t* scope)
{
    for (symbol_t* symbol = symbols[get_symbol_index(type, value, scope)]; symbol != NULL; symbol = symbol->next) {
        if (symbol->type == type && symbol->str == value && symbol->scope == scope) {
            return symbol;
        }
    }

    return NULL;
}

symbol_t* get_symbol_scoped(symbol_type_t type, singleton_t* value, symbol_t* scope)
{
    symbol_t* found = find_symbol(type, value, scope);

    if (found != NULL) {
        return found;
    }

    return create_symbol(type, value, scope);
}

symbol_t* get_symbol(symbol_type_t type, singleton_t* value)
{
    return get_symbol_scoped(type, value, NULL);
}

symbol_t* get_symbol_by_str(symbol_type_t type, const char* str)
{
    return get_symbol(type, get_singleton(str));
}

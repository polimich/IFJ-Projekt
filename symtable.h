//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace symtable.h: xschie03                 //
//    Datum: 11. 10. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include "./symbol.h"

symbol_t* get_symbol(symbol_type_t type, singleton_t* value);

#endif

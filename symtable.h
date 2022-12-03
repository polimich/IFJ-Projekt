//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace symtable.h: xschie03                 //
//    Datum: 02. 12. 2022 - 03. 12. 2022                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#define __STB_CREST__ table, list
#define __STB_DREST__ struct symtable_t *table, struct ast_function_list_t *list

#include "./ast.h"
#include "./symbol.h"

typedef struct symtable_t {
    struct singleton_t* str;
    struct symbol_t* symbol;
    struct symtable_t* lnode;
    struct symtable_t* rnode;
} symtable_t;

void symtable_init(ast_function_list_t* list);

#endif

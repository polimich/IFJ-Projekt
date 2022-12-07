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

#define __STB_CREST__ table, fn, list
#define __STB_DREST__ struct symtable_t *table, struct ast_function_t *fn, struct ast_function_list_t *list

#define STB_PROCESS_DECLARE(type) void symtable_process_##type(ast_##type##_t* type, __STB_DREST__)
#define STB_PROCESS_CALL(type, v) symtable_process_##type(v, __STB_CREST__)

#include "./ast.h"
#include "./symbol.h"

#define SYMTABLE_LOOKUP_SIZE 0x10
#define SYMTABLE_INDEX_MASK 0x0F

STB_PROCESS_DECLARE(node);
STB_PROCESS_DECLARE(block);

typedef struct symtable_t {
    struct singleton_t* str;
    struct symbol_t* symbol;
    struct symtable_t* lnode;
    struct symtable_t* rnode;

    struct symtable_t* lookup[SYMTABLE_LOOKUP_SIZE];
} symtable_t;

void symtable_init(ast_function_list_t* list);

#endif

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

#include "ctype.h"

#include "./ast.h"
#include "./error.h"
#include "./formatter.h"
#include "./memory.h"
#include "./symbol.h"
#include "./symtable.h"
#include "./varstring.h"

typedef enum semantic_type {
    semantic_type_int,
    semantic_type_float,
    semantic_type_string,
    semantic_type_bool,
    semantic_type_dynamic,
} semantic_type_t;

void semantic_check(ast_function_list_t* function_list);
void semantic_check_block(ast_block_t* block, ast_function_list_t* function_list);
semantic_type_t semantic_check_expression(ast_node_t* item, ast_function_list_t* function_list);
semantic_type_t semantic_return_type(ast_function_t* function);
ast_function_t* semantic_check_id(ast_leaf_t* leaf, ast_function_list_t* function_list);
semantic_type_t semantic_constant_type(ast_leaf_t* leaf);

#endif

//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace ast.h: xschie03                      //
//    Datum: 17. 10. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __AST_H__
#define __AST_H__

#include <stdbool.h>

#include "./symbol.h"

typedef struct ast_leaf_t {
    symbol_t* symbol;
} ast_leaf_t;

typedef struct ast_node_t {
    struct ast_leaf_t* leaf;
    struct symbol_t* op;
    struct ast_node_t* left;
    struct ast_node_t* right;
} ast_node_t;

typedef struct ast_block_item_t {
    struct ast_node_t* item;
    struct ast_block_item_t* next;
} ast_block_item_t;

typedef struct ast_block_t {
    size_t num_items;
    struct ast_block_item_t* first;
} ast_block_t;

#endif

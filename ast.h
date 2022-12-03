//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace ast.h: xschie03                      //
//    Datum: 17. 10. 2022 - 21. 11. 2022                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __AST_H__
#define __AST_H__

#include <stdbool.h>

#include "./symbol.h"

typedef struct ast_leaf_t { // constant, local variable or function call
    struct symbol_t* symbol;
    struct ast_call_parameter_list_t* call_parameters;
} ast_leaf_t;

typedef struct ast_node_t {
    struct ast_leaf_t* leaf; // others are null if leaf is not null
    struct symbol_t* op;
    struct ast_node_t* left;
    struct ast_node_t* right;
} ast_node_t;

typedef struct ast_call_parameter_t {
    struct ast_node_t* node;
    struct ast_call_parameter_t* next;
} ast_call_parameter_t;

typedef struct ast_call_parameter_list_t {
    size_t size;
    ast_call_parameter_t* parameters[];
} ast_call_parameter_list_t;

typedef struct ast_conditional_t {
    struct ast_node_t* condition;
    struct ast_block_t* true_branch;
    struct ast_block_t* false_branch;
} ast_conditional_t;

typedef struct ast_loop_t {
    struct ast_node_t* initializer;
    struct ast_node_t* condition;
    struct ast_node_t* incrementer;
    struct ast_block_t* body;
} ast_loop_t;

typedef struct ast_block_item_t {
    struct ast_node_t* item;
    struct ast_conditional_t* conditional;
    struct ast_loop_t* loop;
    struct ast_block_item_t* next;
} ast_block_item_t;

typedef struct ast_block_t {
    size_t num_items;
    struct ast_block_item_t* first;
} ast_block_t;

typedef struct ast_parameter_t {
    struct symbol_t* type;
    struct symbol_t* name;
    struct ast_parameter_t* next;
    bool optional;
} ast_parameter_t;

typedef struct ast_parameter_list_t {
    size_t count;
    ast_parameter_t* parameters[];
} ast_parameter_list_t;

typedef struct ast_function_t {
    struct symbol_t* name;
    struct ast_parameter_list_t* parameters;
    struct ast_block_t* block;
    struct symbol_t* returned_type;
    struct symtable_t* symtable; // local symtable
    bool returned_type_optional;
} ast_function_t;

typedef struct ast_function_list_t {
    struct ast_function_t* item;
    struct ast_function_list_t* next;
} ast_function_list_t;

#endif

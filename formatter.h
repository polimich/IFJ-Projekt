//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace formatter.h: xschie03                //
//    Datum: 28. 11. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __FORMATTER_H__
#define __FORMATTER_H__

#include <stdio.h>

#include "./ast.h"

#define __FMT_CREST__ state, output
#define __FMT_DREST__ formatter_state_t *state, FILE *output

typedef struct formatter_state_t {
    const char* indentation_str;
    int indentation_level;
} formatter_state_t;

void formatter_print_space(__FMT_DREST__);
void formatter_print_indent(__FMT_DREST__);
void formatter_print_singleton(singleton_t* singleton, __FMT_DREST__);
void formatter_print_symbol(symbol_t* symbol, __FMT_DREST__);
void formatter_print_leaf(ast_leaf_t* leaf, __FMT_DREST__);
void formatter_print_expression(ast_node_t* node, __FMT_DREST__);
void formatter_print_statement(ast_node_t* node, __FMT_DREST__);
void formatter_print_conditional(ast_conditional_t* conditional, __FMT_DREST__);
void formatter_print_loop(ast_loop_t* loop, __FMT_DREST__);
void formatter_print_block_item(ast_block_item_t* item, __FMT_DREST__);
void formatter_print_block_contents(ast_block_t* block, __FMT_DREST__);
void formatter_print_block(ast_block_t* block, __FMT_DREST__);
void formatter_print_call_parameter(ast_call_parameter_t* param, __FMT_DREST__);
void formatter_print_call_parameter_list(ast_call_parameter_list_t* list, __FMT_DREST__);
void formatter_print_parameter(ast_parameter_t* param, __FMT_DREST__);
void formatter_print_parameter_list(ast_parameter_list_t* list, __FMT_DREST__);
void formatter_print_function(ast_function_t* fn, __FMT_DREST__);
void formatter_print_function_list(ast_function_list_t* list, __FMT_DREST__);

void formatter(ast_function_list_t* list, const char* indentation_str, FILE* output);

#endif

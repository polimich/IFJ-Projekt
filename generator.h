//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace ast.h: xpoliv07, xmedri01            //
//    Datum: 26. 11. 2022 - x. 11. 2022                 // TODleaO
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
/////////////////////////////////////////////////////////

#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include <stdio.h>

#include "./ast.h"

void generator_print_symbol(symbol_t* symbol, FILE* output);
void generator_print_leaf(ast_leaf_t* leaf, FILE* output);
void generator_print_statement(ast_node_t* node, FILE* output);
void generator_print_conditional(ast_conditional_t* conditional, FILE* output);
void generator_print_loop(ast_loop_t* loop, FILE* output);
void generator_print_block_item(ast_block_item_t* item, FILE* output);
void generator_print_block(ast_block_t* block, FILE* output);
void generator_print_call_parameter(ast_call_parameter_t* param, FILE* output);
void generator_print_call_parameter_list(ast_call_parameter_list_t* list, FILE* output);
void generator_print_parameter(ast_parameter_t* param, FILE* output);
void generator_print_parameter_list(ast_parameter_list_t* list, FILE* output);
void generator_print_function(ast_function_t* fn, FILE* output);
void generator_print_function_list(ast_function_list_t* list, FILE* output);

void generator(ast_function_list_t* list, FILE* output);

#endif
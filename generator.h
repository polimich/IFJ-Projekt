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

#include "./ast.h"
#include "./label.h"
#include <stdio.h>

#define __GEN_CREST__ data, output
#define __GEN_DREST__ generator_data_t *data, FILE *output

typedef struct generator_data_t {
    bool add_generated;
    bool sub_generated;
    bool mul_generated;
    bool div_generated;
} generator_data_t;

void generator_print_constant(symbol_t* constant, __GEN_DREST__);
void generator_print_local_variable(symbol_t* local_variable, __GEN_DREST__);
void generator_print_function_call(ast_leaf_t* function, __GEN_DREST__);
void generator_print_expression(ast_node_t* node, __GEN_DREST__);
void generator_print_assignment(ast_node_t* node, __GEN_DREST__);
void generator_print_operator(symbol_t* op, __GEN_DREST__);
void generator_print_return(ast_node_t* node, __GEN_DREST__);

void generator_print_symbol(symbol_t* symbol, __GEN_DREST__);
void generator_print_leaf(ast_leaf_t* leaf, __GEN_DREST__);
void generator_print_statement(ast_node_t* node, __GEN_DREST__);
void generator_print_conditional(ast_conditional_t* conditional, __GEN_DREST__);
void generator_print_loop(ast_loop_t* loop, __GEN_DREST__);
void generator_print_block_item(ast_block_item_t* item, __GEN_DREST__);
void generator_print_block(ast_block_t* block, __GEN_DREST__);
void generator_print_call_parameter(ast_call_parameter_t* param, __GEN_DREST__);
void generator_print_call_parameter_list(ast_call_parameter_list_t* list, __GEN_DREST__);
void generator_print_parameter(ast_parameter_t* param, __GEN_DREST__);
void generator_print_parameter_list(ast_function_t* function, __GEN_DREST__);
void generator_print_function(ast_function_t* fn, __GEN_DREST__);
void generator_print_function_list(ast_function_list_t* list, __GEN_DREST__);

void generator(ast_function_list_t* list, FILE* output);

#endif
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
//////////////////////////////////////////////////////////

#ifndef __CODE_GENERATOR_H__
#define __CODE_GENERATOR_H__

#include "./ast.h"
#include "./error.h"
#include "./label.h"

void generate_function(ast_function_t* function);
void generate_header();
void interpreter(ast_function_list_t* function_list);
void generate_parameters(ast_parameter_list_t* parameters);
void generate_block(ast_block_t* block);
void generate_node(ast_node_t* node);
void generate_custom_function(ast_function_t* function);
#endif
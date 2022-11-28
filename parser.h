//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace parser.h: xschie03                   //
//    Datum: 17. 10. 2022 - 24. 11. 2022                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>

#include "./ast.h"
#include "./utf8.h"

void parser_init();

ast_node_t* parser_read_paren(utf8_readstream_t* input);
ast_node_t* parser_read_prio1(utf8_readstream_t* input);
ast_node_t* parser_read_prio2(utf8_readstream_t* input);
ast_node_t* parser_read_prio3(utf8_readstream_t* input);
ast_node_t* parser_read_prio4(utf8_readstream_t* input);
ast_node_t* parser_read_prio5(utf8_readstream_t* input);
ast_node_t* parser_read_expression(utf8_readstream_t* input);
ast_block_t* parser_read_block(utf8_readstream_t* input);
ast_function_list_t* parser(utf8_readstream_t* input);

#endif

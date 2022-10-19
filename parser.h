//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace parser.h: xschie03                   //
//    Datum: 17. 10. 2022 - 19. 10. 2022                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>

#include "./ast.h"

typedef enum parser_read_node_until {
    parser_read_node_until_leaf,
    parser_read_node_until_muldiv,
    parser_read_node_until_addsubcon,
    parser_read_node_until_lsgr,
    parser_read_node_until_strict_equals,
    parser_read_node_until_comma,
    parser_read_node_until_semi,
    parser_read_node_until_block_end,
    parser_read_node_entirely,
} parser_read_node_until_t;

ast_node_t* parser_read_node(parser_read_node_until_t read_until, FILE* input);
ast_node_t* parser_fill_node(ast_node_t* node, parser_read_node_until_t read_until, FILE* input);

#endif

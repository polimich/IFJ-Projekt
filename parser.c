//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace parser.c: xschie03                   //
//    Datum: 19. 10. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./parser.h"
#include "./scanner.h"
#include "./symtable.h"

singleton_t* parser_last_singleton = NULL;
singleton_t* parser_next_singleton = NULL;

int parser_last_line_number = 0;
int parser_next_line_number = 0;

singleton_t* parser_read_next_singleton(utf8_readstream_t* input)
{
    parser_last_singleton = parser_next_singleton;
    parser_last_line_number = parser_next_line_number;

    if (parser_last_singleton == NULL) {
        parser_last_singleton = lexer_get_token(input, &parser_last_line_number);
    }

    parser_next_singleton = lexer_get_token(input, &parser_next_line_number);

    return parser_last_singleton;
}

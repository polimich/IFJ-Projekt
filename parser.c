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
#include "./error.h"
#include "./scanner.h"
#include "./symtable.h"

singleton_t* parser_last_singleton = NULL;
singleton_t* parser_next_singleton = NULL;
singleton_t* parser_stashed_singleton = NULL;

int parser_last_line_number = 0;
int parser_next_line_number = 0;
int parser_stashed_line_number = 0;

singleton_t* parser_read_next_singleton(utf8_readstream_t* input)
{
    parser_last_singleton = parser_next_singleton;
    parser_last_line_number = parser_next_line_number;

    if (parser_last_singleton == NULL) {
        parser_last_singleton = lexer_get_token(input, &parser_last_line_number);
    }

    if (parser_stashed_singleton != NULL) {
        parser_next_singleton = parser_stashed_singleton;
        parser_next_line_number = parser_stashed_line_number;
        parser_stashed_line_number = 0;
        parser_stashed_singleton = NULL;
    } else {
        parser_next_singleton = lexer_get_token(input, &parser_next_line_number);
    }

    return parser_last_singleton;
}

void parser_stash_singleton(singleton_t* singleton)
{
    if (parser_stashed_singleton != NULL) {
        throw_error(99, "Parser stashing multiple tokens:\n\tstashed: %s\n\tnext:    %s\n\tnew:     %s", parser_stashed_singleton->strval, parser_next_singleton->strval, singleton->strval);
    }

    parser_stashed_singleton = parser_next_singleton;
    parser_stashed_line_number = parser_next_line_number;

    parser_next_singleton = singleton;
    parser_next_line_number = parser_last_line_number;

    parser_last_singleton = NULL;
    parser_last_line_number = 0;
}

struct reserved_t {
    symbol_t* keyword_else;
    symbol_t* keyword_float;
    symbol_t* keyword_function;
    symbol_t* keyword_if;
    symbol_t* keyword_int;
    symbol_t* keyword_null;
    symbol_t* keyword_return;
    symbol_t* keyword_string;
    symbol_t* keyword_void;
    symbol_t* keyword_while;

    symbol_t* prolog_start;
    symbol_t* prolog_declare;
    symbol_t* prolog_strict_types;

    symbol_t* ending_tag;
};

struct reserved_t reserved = { 0 };

struct operators_t {
    symbol_t* bracket_open;
    symbol_t* bracket_close;
    symbol_t* paren_open;
    symbol_t* paren_close;
    symbol_t* semicolon;
    symbol_t* comma;
    symbol_t* colon;
    symbol_t* questionmark;
    symbol_t* assign;
    symbol_t* SEQ;
    symbol_t* SNEQ;
    symbol_t* GT;
    symbol_t* GTE;
    symbol_t* LT;
    symbol_t* LTE;
    symbol_t* plus;
    symbol_t* minus;
    symbol_t* dot;
    symbol_t* mul;
    symbol_t* div;
};

struct operators_t operators = { 0 };

void parser_init()
{
    reserved.keyword_else = get_symbol_by_str(symbol_type_keyword, "else");
    reserved.keyword_float = get_symbol_by_str(symbol_type_keyword, "float");
    reserved.keyword_function = get_symbol_by_str(symbol_type_keyword, "function");
    reserved.keyword_if = get_symbol_by_str(symbol_type_keyword, "if");
    reserved.keyword_int = get_symbol_by_str(symbol_type_keyword, "int");
    reserved.keyword_null = get_symbol_by_str(symbol_type_keyword, "null");
    reserved.keyword_return = get_symbol_by_str(symbol_type_keyword, "return");
    reserved.keyword_string = get_symbol_by_str(symbol_type_keyword, "string");
    reserved.keyword_void = get_symbol_by_str(symbol_type_keyword, "void");
    reserved.keyword_while = get_symbol_by_str(symbol_type_keyword, "while");

    reserved.prolog_start = get_symbol_by_str(symbol_type_keyword, "<?php");
    reserved.prolog_declare = get_symbol_by_str(symbol_type_keyword, "declare");
    reserved.prolog_strict_types = get_symbol_by_str(symbol_type_keyword, "strict_types");

    reserved.ending_tag = get_symbol_by_str(symbol_type_keyword, "?>");

    operators.bracket_open = get_symbol_by_str(symbol_type_operator, "{");
    operators.bracket_close = get_symbol_by_str(symbol_type_operator, "}");
    operators.paren_open = get_symbol_by_str(symbol_type_operator, "(");
    operators.paren_close = get_symbol_by_str(symbol_type_operator, ")");
    operators.semicolon = get_symbol_by_str(symbol_type_operator, ";");
    operators.comma = get_symbol_by_str(symbol_type_operator, ",");
    operators.colon = get_symbol_by_str(symbol_type_operator, ":");
    operators.questionmark = get_symbol_by_str(symbol_type_operator, "?");
    operators.assign = get_symbol_by_str(symbol_type_operator, "=");
    operators.SEQ = get_symbol_by_str(symbol_type_operator, "===");
    operators.SNEQ = get_symbol_by_str(symbol_type_operator, "!==");
    operators.GT = get_symbol_by_str(symbol_type_operator, ">");
    operators.GTE = get_symbol_by_str(symbol_type_operator, ">=");
    operators.LT = get_symbol_by_str(symbol_type_operator, "<");
    operators.LTE = get_symbol_by_str(symbol_type_operator, "<=");
    operators.plus = get_symbol_by_str(symbol_type_operator, "+");
    operators.minus = get_symbol_by_str(symbol_type_operator, "-");
    operators.dot = get_symbol_by_str(symbol_type_operator, ".");
    operators.mul = get_symbol_by_str(symbol_type_operator, "*");
    operators.div = get_symbol_by_str(symbol_type_operator, "/");
}

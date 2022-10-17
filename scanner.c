//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace scanner.c: xpoliv07, xlukas18        //
//    Datum: 7. 10. 2022 - 16. 10. 2022                 //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./scanner.h"

enum lexer_state {
    lexer_state_start,
    lexer_state_identifier,
    lexer_state_var0,
    lexer_state_var_id,
    lexer_state_number,
    lexer_state_number_exponent,
    lexer_state_number_exponent_sign,
    lexer_state_number_double,
    lexer_state_number_point,
    lexer_state_number_exponent_final,
    lexer_state_div,
    lexer_state_assign,
    lexer_state_equals0,
    lexer_state_greater,
    lexer_state_lesser,
    lexer_state_not,
    lexer_state_not_equal0,
    lexer_state_line_comment,
    lexer_state_block_comment,
    lexer_state_block_comment_end,
    lexer_state_string,
    lexer_state_string_literal,
    lexer_state_string_esc,
    lexer_state_question_mark,
    lexer_state_type,
    lexer_state_prolog0,
};

#define putback(c)        \
    if (c != EOF) {       \
        ungetc(c, input); \
    }

singleton_t* lexer_get_token(FILE* input)
{
    int lexer_state = lexer_state_start;
    int c = 0;

    varstring_t* identifier = NULL;

    while ((c = getc(input)), 1) {
        switch (lexer_state) {
        case lexer_state_start:
            switch (c) {
            case EOF:
                return NULL;
            case '/':
                lexer_state = lexer_state_div;
                continue;
            case ';':
                return get_singleton(";");
            case ',':
                return get_singleton(",");
            case '(':
                return get_singleton("(");
            case ')':
                return get_singleton(")");
            case '*':
                return get_singleton("*");
            case '+':
                return get_singleton("+");
            case '-':
                return get_singleton("-");
            case '=':
                lexer_state = lexer_state_assign;
                continue;
            case '>':
                lexer_state = lexer_state_greater;
                continue;
            case '<':
                lexer_state = lexer_state_lesser;
                continue;
            case '\"':
                lexer_state = lexer_state_string;
                identifier = varstring_init();
                putc(c, identifier->stream);
                continue;
            case '$':
                lexer_state = lexer_state_var0;
                identifier = varstring_init();
                putc(c, identifier->stream);
                continue;
            case ':':
                return get_singleton(":");
            case '{':
                return get_singleton("{");
            case '}':
                return get_singleton("}");
            case '!':
                lexer_state = lexer_state_not;
                continue;
            case '.':
                return get_singleton(".");
            case '?':
                lexer_state = lexer_state_question_mark;
                identifier = varstring_init();
                putc(c, identifier->stream);
                continue;
            default:
                if (isalpha(c) || c == '_') {
                    lexer_state = lexer_state_identifier;
                    identifier = varstring_init();
                    putc(c, identifier->stream);
                } else if (isdigit(c)) {
                    lexer_state = lexer_state_number;
                    identifier = varstring_init();
                    putc(c, identifier->stream);
                }
                continue;
            }
        case lexer_state_div:
            switch (c) {
            case '/':
                lexer_state = lexer_state_line_comment;
                continue;
            case '*':
                lexer_state = lexer_state_block_comment;
                continue;
            default:
                putback(c);
                return get_singleton("/");
            }
        case lexer_state_line_comment:
            if (c != '\n') {
                continue;
            } else {
                lexer_state = lexer_state_start;
                continue;
            }
        case lexer_state_block_comment:
            if (c == '*') {
                lexer_state = lexer_state_block_comment_end;
                continue;
            } else {
                continue;
            }
        case lexer_state_block_comment_end:
            if (c == '/') {
                lexer_state = lexer_state_start;
                continue;
            } else {
                lexer_state = lexer_state_block_comment;
                continue;
            }

        case lexer_state_identifier:
            if (isalnum(c) || c == '_') {
                putc(c, identifier->stream);
                continue;
            } else {
                putback(c);
                return varstring_destroy(identifier);
            }
        case lexer_state_var0:
            if (isalpha(c) || c == '_') {
                lexer_state = lexer_state_var_id;
                putc(c, identifier->stream);
                continue;
            }
            else {
                varstring_destroy(identifier);
                exit(1);
            }
        case lexer_state_var_id:
            if (isalnum(c) || c == '_') {
                lexer_state = lexer_state_var_id;
                putc(c, identifier->stream);
                continue;
            } else {
                putback(c);
                return varstring_destroy(identifier);
            }

        case lexer_state_number:
            if (isdigit(c)) {
                putc(c, identifier->stream);
                continue;
            } else if (c == '.') {
                putc(c, identifier->stream);
                lexer_state = lexer_state_number_point;
                continue;
            } else if (c == 'e' || c == 'E') {
                putc(c, identifier->stream);
                lexer_state = lexer_state_number_exponent;
                continue;
            } else {
                lexer_state = lexer_state_start;
                putback(c);
                return varstring_destroy(identifier);
            }
        case lexer_state_number_point:
            if (isdigit(c)) {
                putc(c, identifier->stream);
                lexer_state = lexer_state_number_double;
                continue;
            } else {
                varstring_destroy(identifier);
                exit(1);
            }

        case lexer_state_number_double:
            if (isdigit(c)) {
                putc(c, identifier->stream);
                continue;
            } else if (c == 'e' || c == 'E') {
                putc(c, identifier->stream);
                lexer_state = lexer_state_number_exponent;
                continue;
            } else {
                putback(c);
                lexer_state = lexer_state_start;
                return varstring_destroy(identifier);
            }
        case lexer_state_number_exponent:
            if (isdigit(c)) {
                putc(c, identifier->stream);
                lexer_state = lexer_state_number_exponent_final;
                continue;
            } else if (c == '+' || c == '-') {
                putc(c, identifier->stream);
                lexer_state = lexer_state_number_exponent_sign;
                continue;
            } else {
                putback(c);
                return varstring_destroy(identifier);
            }
        case lexer_state_number_exponent_sign:
            if (isdigit(c)) {
                putc(c, identifier->stream);
                lexer_state = lexer_state_number_exponent_final;
                continue;
            } else {
                varstring_destroy(identifier);
                exit(1);
            }
        case lexer_state_number_exponent_final:
            if (isdigit(c)) {
                putc(c, identifier->stream);
                continue;
            } else {
                putback(c);
                return varstring_destroy(identifier);
            }
        case lexer_state_assign:
            if (c == '=') {
                lexer_state = lexer_state_equals0;
                continue;
            } else {
                putback(c);
                return get_singleton("=");
            }

        case lexer_state_equals0:
            if (c == '=') {
                return get_singleton("===");

            } else {
                exit(1);
            }
        case lexer_state_not:
            if (c == '=') {
                lexer_state = lexer_state_not_equal0;
                continue;
            } else {
                putback(c);
                return get_singleton("!");
            }
        case lexer_state_not_equal0:
            if (c == '=') {
                return get_singleton("!==");
            } else {
                exit(1);
            }

        case lexer_state_string:
            if (c == '\"') {
                putc(c, identifier->stream);
                lexer_state = lexer_state_string_literal;
                continue;
            } else if (c == '\\') {
                putc(c, identifier->stream);
                lexer_state = lexer_state_string_esc;
                continue;
            } else {
                putc(c, identifier->stream);
                continue;
            }
        case lexer_state_string_literal:
            putback(c);
            return varstring_destroy(identifier);
        case lexer_state_string_esc:
            putc(c, identifier->stream);
            lexer_state = lexer_state_string;
            continue;

        case lexer_state_greater:
            if (c == '=') {
                return get_singleton(">=");
            } else {
                putback(c);
                return get_singleton(">");
            }

        case lexer_state_lesser:
            if (c == '=') {
                return get_singleton("<=");
            } else if (c == '?') {
                lexer_state = lexer_state_prolog0;
                identifier = varstring_init();
                putc('<', identifier->stream);
                putc(c, identifier->stream);
                continue;
            } else {
                putback(c);
                return get_singleton("<");
            }

        case lexer_state_question_mark:
            if (c == '>') {
                putc(c, identifier->stream);
                return varstring_destroy(identifier);

            } else if (isalpha(c)) {
                lexer_state = lexer_state_type;
                putc(c, identifier->stream);
                continue;
            } else {
                varstring_destroy(identifier);
                exit(1);
            }
        case lexer_state_type:
            if (isalpha(c)) {
                putc(c, identifier->stream);
                continue;
            } else {
                putback(c);
                return varstring_destroy(identifier);
            }
            /*
            case lexer_state_prolog0:
                for (int i = 0; i < 3; i++) {
                    c = getc(input);
                    putc(c, identifier->stream);
                }
                if (identifier == "<?php") {
                }
            */
        }
    }
}


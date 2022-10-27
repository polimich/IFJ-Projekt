//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace scanner.c: xpoliv07, xlukas18        //
//    Datum: 7. 10. 2022 - 27. 10. 2022                 //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./scanner.h"
#include "./error.h"
#include "./utf8.h"

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
};

#define putback(c)               \
    if (c != EOF && c != '\n') { \
        utf8_ungetc(c, input);   \
    }

int line_counter = 1;

singleton_t* lexer_get_token(utf8_readstream_t* input, int* line_number)
{
    int lexer_state = lexer_state_start;
    int c = 0;

    varstring_t* identifier = NULL;

    *line_number = line_counter;

    while ((c = utf8_getc(input)), 1) {
        if (c == '\n') {
            ++line_counter;
        }

        switch (lexer_state) {
        case lexer_state_start:
            switch (c) {
            case EOF:
                return get_singleton("?>");
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
                utf8_putc(c, identifier->stream);
                continue;
            case '$':
                lexer_state = lexer_state_var0;
                identifier = varstring_init();
                utf8_putc(c, identifier->stream);
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
                utf8_putc(c, identifier->stream);
                continue;
            case '\n':
                *line_number = line_counter;
                continue;
            case ' ':
            case '\t':
            case '\v':
            case '\r':
            case '\f':
                continue;
            default:
                if (utf8_isalpha(c) || c == '_') {
                    lexer_state = lexer_state_identifier;
                    identifier = varstring_init();
                    utf8_putc(c, identifier->stream);
                } else if (utf8_isdigit(c)) {
                    lexer_state = lexer_state_number;
                    identifier = varstring_init();
                    utf8_putc(c, identifier->stream);
                } else if (c < ' ') {
                    throw_warning(1, "Unexpected '\\%o' on line %d", c, line_counter);
                } else {
                    throw_warning(1, "Unexpected '%s' on line %d", utf8_encode_int(c), line_counter);
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
            if (c != '\n' && c != EOF) {
                continue;
            } else {
                lexer_state = lexer_state_start;
                continue;
            }
        case lexer_state_block_comment:
            if (c == '*') {
                lexer_state = lexer_state_block_comment_end;
                continue;
            } else if (c == EOF) {
                throw_warning(1, "Unterminated block comment starting on line %d", *line_number);
                lexer_state = lexer_state_start;
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
            if (utf8_isalnum(c) || c == '_') {
                utf8_putc(c, identifier->stream);
                continue;
            } else {
                putback(c);
                return varstring_destroy(identifier);
            }
        case lexer_state_var0:
            if (utf8_isalpha(c) || c == '_') {
                utf8_putc(c, identifier->stream);
            } else if (c == EOF) {
                throw_warning(1, "Unexpected end-of-file after '$' on line %d", line_counter);
            } else {
                throw_warning(1, "Variable name cannot start with '%s' (line %d)", utf8_encode_int(c), line_counter);
                varstring_write(identifier, "_0x%x_", c);
            }
            lexer_state = lexer_state_var_id;
            continue;
        case lexer_state_var_id:
            if (utf8_isalnum(c) || c == '_') {
                lexer_state = lexer_state_var_id;
                utf8_putc(c, identifier->stream);
                continue;
            } else {
                putback(c);
                return varstring_destroy(identifier);
            }

        case lexer_state_number:
            if (utf8_isdigit(c)) {
                utf8_putc(c, identifier->stream);
                continue;
            } else if (c == '.') {
                utf8_putc(c, identifier->stream);
                lexer_state = lexer_state_number_point;
                continue;
            } else if (c == 'e' || c == 'E') {
                utf8_putc(c, identifier->stream);
                lexer_state = lexer_state_number_exponent;
                continue;
            } else {
                lexer_state = lexer_state_start;
                putback(c);
                return varstring_destroy(identifier);
            }
        case lexer_state_number_point:
            if (utf8_isdigit(c)) {
                utf8_putc(c, identifier->stream);
                lexer_state = lexer_state_number_double;
                continue;
            } else {
                varstring_destroy(identifier);
                exit(1);
            }

        case lexer_state_number_double:
            if (utf8_isdigit(c)) {
                utf8_putc(c, identifier->stream);
                continue;
            } else if (c == 'e' || c == 'E') {
                utf8_putc(c, identifier->stream);
                lexer_state = lexer_state_number_exponent;
                continue;
            } else {
                putback(c);
                lexer_state = lexer_state_start;
                return varstring_destroy(identifier);
            }
        case lexer_state_number_exponent:
            if (utf8_isdigit(c)) {
                utf8_putc(c, identifier->stream);
                lexer_state = lexer_state_number_exponent_final;
                continue;
            } else if (c == '+' || c == '-') {
                utf8_putc(c, identifier->stream);
                lexer_state = lexer_state_number_exponent_sign;
                continue;
            } else {
                putback(c);
                return varstring_destroy(identifier);
            }
        case lexer_state_number_exponent_sign:
            if (utf8_isdigit(c)) {
                utf8_putc(c, identifier->stream);
                lexer_state = lexer_state_number_exponent_final;
                continue;
            } else {
                varstring_destroy(identifier);
                exit(1);
            }
        case lexer_state_number_exponent_final:
            if (utf8_isdigit(c)) {
                utf8_putc(c, identifier->stream);
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
                throw_warning(1, "== operator not supported, assuming ===");
                putback(c);

                return get_singleton("===");
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
                utf8_putc(c, identifier->stream);
                lexer_state = lexer_state_string_literal;
                continue;
            } else if (c == '\\') {
                utf8_putc(c, identifier->stream);
                lexer_state = lexer_state_string_esc;
                continue;
            } else {
                utf8_putc(c, identifier->stream);
                continue;
            }
        case lexer_state_string_literal:
            putback(c);
            return varstring_destroy(identifier);
        case lexer_state_string_esc:
            utf8_putc(c, identifier->stream);
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
                lexer_state = lexer_state_identifier; // ocekavame <?php
                identifier = varstring_init();
                utf8_putc('<', identifier->stream);
                utf8_putc(c, identifier->stream);
                continue;
            } else {
                putback(c);
                return get_singleton("<");
            }

        case lexer_state_question_mark:
            if (c == '>') {
                utf8_putc(c, identifier->stream);
                return varstring_destroy(identifier);

            } else if (utf8_isalpha(c)) {
                lexer_state = lexer_state_type;
                utf8_putc(c, identifier->stream);
                continue;
            } else {
                varstring_destroy(identifier);
                exit(1);
            }
        case lexer_state_type:
            if (utf8_isalpha(c)) {
                utf8_putc(c, identifier->stream);
                continue;
            } else {
                putback(c);
                return varstring_destroy(identifier);
            }
        }
    }
}

//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace parser.c: xschie03                   //
//    Datum: 19. 10. 2022 - 03. 12. 2022                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./parser.h"
#include "./error.h"
#include "./formatter.h"
#include "./scanner.h"
#include "./symtable.h"

static singleton_t* parser_last_singleton = NULL;
static singleton_t* parser_next_singleton = NULL;
static singleton_t* parser_stashed_singleton = NULL;

static int parser_last_line_number = 0;
static int parser_next_line_number = 0;
static int parser_stashed_line_number = 0;

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
    symbol_t* boolean_true;
    symbol_t* boolean_false;

    symbol_t* keyword_bool;
    symbol_t* keyword_else;
    symbol_t* keyword_elseif;
    symbol_t* keyword_float;
    symbol_t* keyword_for;
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

static struct reserved_t reserved = { 0 };

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

static struct operators_t operators = { 0 };

void parser_init()
{
    reserved.boolean_true = get_symbol_by_str(symbol_type_keyword, "true");
    reserved.boolean_false = get_symbol_by_str(symbol_type_keyword, "false");

    reserved.keyword_bool = get_symbol_by_str(symbol_type_keyword, "bool");
    reserved.keyword_else = get_symbol_by_str(symbol_type_keyword, "else");
    reserved.keyword_elseif = get_symbol_by_str(symbol_type_keyword, "elseif");
    reserved.keyword_float = get_symbol_by_str(symbol_type_keyword, "float");
    reserved.keyword_for = get_symbol_by_str(symbol_type_keyword, "for");
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

void parser_check_headers(utf8_readstream_t* input)
{
    int first_character = utf8_getc(input);

    if (first_character != '<') {
        throw_warning(1, "Program does not begin with <?php");
    }

    utf8_ungetc(first_character, input);

    singleton_t* prolog_tag = parser_read_next_singleton(input);

    if (prolog_tag != reserved.prolog_start->str) {
        throw_warning(2, "Prolog: expected '<?php', received '%s'", prolog_tag->strval);

        parser_stash_singleton(prolog_tag);
    }

    singleton_t* prolog_declaration[] = {
        reserved.prolog_declare->str,
        operators.paren_open->str,
        reserved.prolog_strict_types->str,
        operators.assign->str,
        get_singleton("1"),
        operators.paren_close->str,
        operators.semicolon->str,
    };

    for (int i = 0; i < 7; ++i) {
        if (parser_next_singleton == prolog_declaration[i]) {
            parser_read_next_singleton(input);
        } else {
            throw_warning(2, "Prolog: expected '%s', received '%s'", prolog_declaration[i]->strval, parser_next_singleton->strval);
        }
    }
}

void parser_skip_semicolons(utf8_readstream_t* input)
{
    while (parser_next_singleton == operators.semicolon->str) {
        parser_read_next_singleton(input);
    }
}

ast_call_parameter_t* parser_read_call_parameter(utf8_readstream_t* input)
{
    salloc(ast_call_parameter_t, parameter);

    parameter->node = parser_read_expression(input);

    return parameter;
}

ast_call_parameter_list_t* parser_read_call_parameters(utf8_readstream_t* input)
{
    size_t count = 0;
    ast_call_parameter_t* param = NULL;
    ast_call_parameter_t** next = &param;

    singleton_t* paren_open = parser_read_next_singleton(input);

    runtime_assert(
        paren_open == operators.paren_open->str,
        99,
        "parser_read_call_parameters() expected (, received '%s'",
        paren_open->strval);

    while (parser_next_singleton != operators.paren_close->str) {
        *next = parser_read_call_parameter(input);
        next = &((*next)->next);

        count++;

        if (parser_next_singleton == operators.paren_close->str) {
            break;
        } else if (parser_next_singleton == operators.comma->str) {
            parser_read_next_singleton(input);

            if (parser_next_singleton == operators.paren_close->str) {
                throw_warning(2, "Trailing commas are not allowed! (line %d)", parser_last_line_number);
            }
        } else {
            throw_warning(2, "Missing comma in function parameters on line %d: %s %s", parser_last_line_number, parser_last_singleton->strval, parser_next_singleton->strval);
        }
    }

    ast_call_parameter_list_t* parameter_list = mem_alloc_safe(sizeof(ast_call_parameter_list_t) + sizeof(ast_call_parameter_t*) * count);

    for (size_t i = 0; i < count; ++i, param = param->next) {
        parameter_list->parameters[i] = param;
    }

    parameter_list->size = count;

    singleton_t* paren_close = parser_read_next_singleton(input);

    runtime_assert(
        paren_close == operators.paren_close->str,
        99,
        "parser_read_call_parameters() expected ), received '%s'",
        paren_close->strval);

    return parameter_list;
}

symbol_t* parser_read_undefined_symbol(utf8_readstream_t* input)
{
    symbol_t* symbol = get_symbol(symbol_type_undefined, parser_read_next_singleton(input));

    symbol->line_number = parser_last_line_number;

    return symbol;
}

ast_node_t* parser_read_leaf(utf8_readstream_t* input)
{
    salloc(ast_node_t, node);
    salloc(ast_leaf_t, leaf);

    leaf->symbol = parser_read_undefined_symbol(input);
    node->leaf = leaf;

    if (parser_next_singleton == operators.paren_open->str) {
        node->leaf->call_parameters = parser_read_call_parameters(input);
    }

    return node;
}

ast_node_t* parser_read_paren(utf8_readstream_t* input)
{
    singleton_t* lparen = parser_read_next_singleton(input);

    bool expecting_closing_paren = true;

    if (lparen != operators.paren_open->str) {
        throw_warning(2, "Expected '(', received '%s'", lparen->strval);
        parser_stash_singleton(lparen);
        expecting_closing_paren = false;
    }

    ast_node_t* node = parser_read_expression(input);

    if (expecting_closing_paren) {
        singleton_t* rparen = parser_read_next_singleton(input);

        if (rparen != operators.paren_close->str) {
            throw_warning(2, "Expected ')', received '%s'", rparen->strval);
            parser_stash_singleton(rparen);
        }
    }

    return node;
}

ast_node_t* parser_read_prio1(utf8_readstream_t* input)
{
    ast_node_t* left = (parser_next_singleton == operators.paren_open->str) ? parser_read_paren(input) : parser_read_leaf(input);

    if (parser_next_singleton == operators.mul->str || parser_next_singleton == operators.div->str) {
        salloc(ast_node_t, node);

        node->left = left;
        node->op = get_symbol(symbol_type_operator, parser_read_next_singleton(input));
        node->right = parser_read_prio1(input);

        return node;
    } else {
        return left;
    }
}

ast_node_t* parser_read_prio2(utf8_readstream_t* input)
{
    ast_node_t* left = parser_read_prio1(input);

    if (parser_next_singleton == operators.plus->str || parser_next_singleton == operators.minus->str || parser_next_singleton == operators.dot->str) {
        salloc(ast_node_t, node);

        node->left = left;
        node->op = get_symbol(symbol_type_operator, parser_read_next_singleton(input));
        node->right = parser_read_prio2(input);

        return node;
    } else {
        return left;
    }
}

ast_node_t* parser_read_prio3(utf8_readstream_t* input)
{
    ast_node_t* left = parser_read_prio2(input);

    if (parser_next_singleton == operators.LT->str || parser_next_singleton == operators.LTE->str || parser_next_singleton == operators.GT->str || parser_next_singleton == operators.GTE->str) {
        salloc(ast_node_t, node);

        node->left = left;
        node->op = get_symbol(symbol_type_operator, parser_read_next_singleton(input));
        node->right = parser_read_prio3(input);

        return node;
    } else {
        return left;
    }
}

ast_node_t* parser_read_prio4(utf8_readstream_t* input)
{
    ast_node_t* left = parser_read_prio3(input);

    if (parser_next_singleton == operators.SEQ->str || parser_next_singleton == operators.SNEQ->str) {
        salloc(ast_node_t, node);

        node->left = left;
        node->op = get_symbol(symbol_type_operator, parser_read_next_singleton(input));
        node->right = parser_read_prio4(input);

        return node;
    } else {
        return left;
    }
}

bool parser_check_lvalue(ast_node_t* lvalue)
{
    if (lvalue->op || lvalue->left || lvalue->right) {
        return false;
    }

    if (!lvalue->leaf) {
        return false;
    }

    if (lvalue->leaf->symbol->str->strval[0] != '$') {
        return false;
    }

    if (lvalue->leaf->call_parameters) {
        return false;
    }

    return true;
}

ast_node_t* parser_read_prio5(utf8_readstream_t* input)
{
    ast_node_t* left = parser_read_prio4(input);

    if (parser_next_singleton == operators.assign->str) {
        if (!parser_check_lvalue(left)) {
            varstring_t* error_message = varstring_init();

            varstring_write(error_message, "Cannot assign to ");

            formatter_state_t state = { 0 };
            formatter_print_expression(left, &state, error_message->stream);

            varstring_write(error_message, " on line %d", parser_last_line_number);

            throw_error(2, "%s", varstring_destroy(error_message)->strval);
        }

        salloc(ast_node_t, node);

        node->left = left;
        node->op = get_symbol(symbol_type_operator, parser_read_next_singleton(input));
        node->right = parser_read_prio5(input);

        return node;
    } else {
        return left;
    }
}

ast_node_t* parser_read_expression(utf8_readstream_t* input)
{
    return parser_read_prio5(input);
}

ast_node_t* parser_read_statement(utf8_readstream_t* input)
{
    ast_node_t* node = parser_read_expression(input);

    if (parser_next_singleton == operators.semicolon->str) {
        parser_read_next_singleton(input);
    } else {
        throw_warning(2, "Missing semicolon on line %d", parser_last_line_number);
    }

    return node;
}

ast_conditional_t* parser_read_if(utf8_readstream_t* input)
{
    if (parser_next_singleton == reserved.keyword_if->str) {
        parser_read_next_singleton(input);
    } else {
        throw_warning(99, "Called parser_read_if() on non-if on line %d", parser_next_line_number);
    }

    salloc(ast_conditional_t, statement);

    statement->condition = parser_read_expression(input);
    statement->true_branch = parser_read_block(input);

    if (parser_next_singleton == reserved.keyword_else->str) {
        parser_read_next_singleton(input);
        statement->false_branch = parser_read_block(input);
    } else if (parser_next_singleton == reserved.keyword_elseif->str) {
        parser_next_singleton = reserved.keyword_if->str;
        statement->false_branch = parser_read_block(input);
    }

    return statement;
}

ast_loop_t* parser_read_while(utf8_readstream_t* input)
{
    if (parser_next_singleton == reserved.keyword_while->str) {
        parser_read_next_singleton(input);
    } else {
        throw_warning(99, "Called parser_read_while() on non-while on line %d", parser_next_line_number);
    }

    salloc(ast_loop_t, statement);

    statement->condition = parser_read_expression(input);
    statement->body = parser_read_block(input);

    return statement;
}

ast_loop_t* parser_read_for(utf8_readstream_t* input)
{
    if (parser_next_singleton == reserved.keyword_for->str) {
        parser_read_next_singleton(input);
    } else {
        throw_warning(99, "Called parser_read_for() on non-for on line %d", parser_next_line_number);
    }

    salloc(ast_loop_t, statement);

    bool has_paren_condition = parser_next_singleton == operators.paren_open->str;

    if (has_paren_condition) {
        parser_read_next_singleton(input);
    }

    if (parser_next_singleton == operators.semicolon->str) {
        statement->initializer = NULL;
        parser_read_next_singleton(input);
    } else {
        statement->initializer = parser_read_statement(input);
    }

    if (parser_next_singleton == operators.semicolon->str) {
        statement->condition = NULL;
        parser_read_next_singleton(input);
    } else {
        statement->condition = parser_read_statement(input);
    }

    if (parser_next_singleton == operators.paren_close->str) {
        statement->incrementer = NULL;
    } else {
        statement->incrementer = parser_read_expression(input);
    }

    if (has_paren_condition) {
        if (parser_next_singleton == operators.paren_close->str) {
            parser_read_next_singleton(input);
        } else {
            throw_warning(2, "Missing ')' in for loop on line %d", parser_last_line_number);
        }
    }

    statement->body = parser_read_block(input);

    return statement;
}

ast_block_item_t* parser_read_block_item(utf8_readstream_t* input)
{
    salloc(ast_block_item_t, item);

    int errmsg_return_line_number = parser_last_line_number;

    if (parser_next_singleton == reserved.keyword_return->str) {
        parser_read_next_singleton(input); // discard 'return'
        item->is_return_statement = true;

        if (parser_next_singleton == operators.semicolon->str) {
            parser_stash_singleton(get_singleton("null"));
        }
    }

    if (parser_next_singleton == reserved.keyword_if->str) {
        item->conditional = parser_read_if(input);
    } else if (parser_next_singleton == reserved.keyword_for->str) {
        item->loop = parser_read_for(input);
    } else if (parser_next_singleton == reserved.keyword_while->str) {
        item->loop = parser_read_while(input);
    } else {
        item->item = parser_read_statement(input);
    }

    if (item->is_return_statement && !item->item) {
        throw_warning(2, "SyntaxError: cannot return %s (line %d)", item->conditional ? "conditional statement" : "loop", errmsg_return_line_number);
    }

    return item;
}

ast_block_t* parser_read_block(utf8_readstream_t* input)
{
    if (parser_next_singleton != operators.bracket_open->str) {
        salloc(ast_block_t, block);

        block->num_items = 1;
        block->first = parser_read_block_item(input);

        return block;
    }

    parser_read_next_singleton(input); // '{'

    salloc(ast_block_t, block);
    ast_block_item_t** next = &block->first;

    while (parser_next_singleton != operators.bracket_close->str) {
        parser_skip_semicolons(input);

        *next = parser_read_block_item(input);
        next = &((*next)->next);
        ++block->num_items;

        parser_skip_semicolons(input);
    }

    parser_read_next_singleton(input); // '}'

    return block;
}

symbol_t* parser_read_function_identifier(utf8_readstream_t* input)
{
    singleton_t* singleton = parser_read_next_singleton(input);
    symbol_t* symbol = get_symbol(symbol_type_function_identifier, singleton);
    const char* name = singleton->strval;

    symbol->line_number = parser_last_line_number;

    if (!utf8_isalpha(name[0]) && (name[0] != '_')) {
        throw_error(2, "Invalid function name: '%s'", name);
    }

    return symbol;
}

symbol_t* parser_read_type(utf8_readstream_t* input)
{
    if (parser_next_singleton == reserved.keyword_bool->str) {
        parser_read_next_singleton(input);

        return reserved.keyword_bool;
    } else if (parser_next_singleton == reserved.keyword_float->str) {
        parser_read_next_singleton(input);

        return reserved.keyword_float;
    } else if (parser_next_singleton == reserved.keyword_int->str) {
        parser_read_next_singleton(input);

        return reserved.keyword_int;
    } else if (parser_next_singleton == reserved.keyword_string->str) {
        parser_read_next_singleton(input);

        return reserved.keyword_string;
    } else if (parser_next_singleton == reserved.keyword_void->str) {
        parser_read_next_singleton(input);

        return reserved.keyword_void;
    } else {
        throw_warning(2, "Invalid type: '%s'", parser_next_singleton->strval);

        return reserved.keyword_void;
    }
}

ast_parameter_t* parser_read_parameter(utf8_readstream_t* input)
{
    salloc(ast_parameter_t, parameter);

    if (parser_next_singleton == operators.questionmark->str) {
        parameter->optional = true;
        parser_read_next_singleton(input);
    }

    parameter->type = parser_read_type(input);
    parameter->name = get_symbol(symbol_type_local_variable, parser_read_next_singleton(input));

    parameter->name->line_number = parser_last_line_number;

    return parameter;
}

ast_parameter_list_t* parser_read_parameter_list(utf8_readstream_t* input)
{
    if (parser_next_singleton == operators.paren_open->str) {
        parser_read_next_singleton(input);
    } else {
        throw_warning(2, "Expecting '(' after function name, received '%s'", parser_next_singleton->strval);

        salloc(ast_parameter_list_t, empty);

        return empty;
    }

    ast_parameter_t* first = NULL;
    ast_parameter_t** next = &first;

    size_t count = 0;

    while (parser_next_singleton != operators.paren_close->str) {
        *next = parser_read_parameter(input);
        next = &((*next)->next);
        ++count;

        if (parser_next_singleton == operators.comma->str) {
            parser_read_next_singleton(input);

            if (parser_next_singleton == operators.paren_close->str) {
                throw_warning(2, "Trailing commas are not allowed! (line %d)", parser_last_line_number);
            }
        } else if (parser_next_singleton != operators.paren_close->str) {
            throw_warning(2, "Missing comma between '%s' and '%s' on line %d", parser_last_singleton->strval, parser_next_singleton->strval, parser_next_line_number);
        }

        if (parser_next_singleton == reserved.ending_tag->str) {
            throw_warning(2, "Unterminated function parameters on line %d", parser_last_line_number);

            break;
        }
    }

    parser_read_next_singleton(input); // ')'

    ast_parameter_list_t* list = mem_alloc_safe(sizeof(ast_parameter_list_t) + count * sizeof(ast_call_parameter_t*));

    ast_parameter_t* ptr = first;
    for (size_t i = 0; ptr != NULL && i < count; ptr = ptr->next) {
        list->parameters[i++] = ptr;
    }

    list->count = count;

    return list;
}

ast_function_t* parser_read_function(utf8_readstream_t* input)
{
    if (parser_next_singleton == reserved.keyword_function->str) {
        parser_read_next_singleton(input);
    } else {
        throw_warning(99, "parser_read_function() called on non-function");
    }

    salloc(ast_function_t, fn);

    fn->name = parser_read_function_identifier(input);
    fn->parameters = parser_read_parameter_list(input);

    if (parser_next_singleton == operators.colon->str) {
        parser_read_next_singleton(input); // ':'

        if (parser_next_singleton == operators.questionmark->str) {
            parser_read_next_singleton(input); // '?'
            fn->returned_type_optional = true;
        } else {
            fn->returned_type_optional = false;
        }

        fn->returned_type = parser_read_type(input);
    } else {
        throw_warning(2, "The function %s should specify a return type.", fn->name->str->strval);

        fn->returned_type = reserved.keyword_void;
    }

    fn->block = parser_read_block(input);

    return fn;
}

ast_function_list_t* parser_read_function_list(utf8_readstream_t* input)
{
    salloc(ast_function_list_t, list);
    salloc(ast_function_t, main);

    list->item = main;

    ast_function_list_t* current_list = list;

    salloc(ast_block_t, main_block);
    ast_block_item_t** main_next_expr_ptr = &(main_block->first);

    main->block = main_block;

    while (parser_next_singleton != reserved.ending_tag->str) {
        parser_skip_semicolons(input);

        if (parser_next_singleton == reserved.keyword_function->str) {
            salloc(ast_function_list_t, next_list);

            next_list->item = parser_read_function(input);

            current_list->next = next_list;
            current_list = next_list;
        } else {
            ast_block_item_t* item = parser_read_block_item(input);

            *main_next_expr_ptr = item;
            main_next_expr_ptr = &(item->next);

            ++main_block->num_items;
        }

        parser_skip_semicolons(input);
    }

    return list;
}

ast_function_list_t* parser(utf8_readstream_t* input)
{
    parser_init();
    parser_check_headers(input);

    return parser_read_function_list(input);
}

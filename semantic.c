//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace semantic.c: xlukas18                 //
//    Datum: 28. 11. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./semantic.h"

int semantic_get_line_number(ast_node_t* node) {
    if (node->leaf != NULL){
        return node->leaf->symbol->line_number;
    }
    else{
        return semantic_get_line_number(node->left);
    }
}

size_t semantic_buildin_parameter_count(ast_leaf_t* leaf){
    if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "write")) return -1;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "reads")) return 0; 
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "readf")) return 0;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "readi")) return 0;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "strval")) return 1;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "floatval")) return 1;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "intval")) return 1;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "strlen")) return 1;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "ord")) return 1;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "chr")) return 1;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "substring")) return 3;
    else return 0;  // can not happen
}

semantic_type_t semantic_buildin_return_type (ast_leaf_t* leaf){
    if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "write")) return semantic_type_null;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "reads")) return semantic_type_string;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "readf")) return semantic_type_float;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "readi")) return semantic_type_int;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "strval")) return semantic_type_string;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "floatval")) return semantic_type_float;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "intval")) return semantic_type_int;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "strlen")) return semantic_type_int;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "ord")) return semantic_type_int;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "chr")) return semantic_type_string;
    else if (leaf->symbol == get_symbol_by_str(symbol_type_keyword, "substring")) return semantic_type_string;
    else return semantic_type_dynamic;  // can not happen
}

semantic_type_t semantic_constant_type(ast_leaf_t* leaf)
{
    if (!strcmp(leaf->symbol->str->strval, "true") || !strcmp(leaf->symbol->str->strval, "false")) {
        // bool
        symbol_t* new_symbol = get_symbol(symbol_type_constant, leaf->symbol->str);

        new_symbol->line_number = leaf->symbol->line_number;
        leaf->symbol = new_symbol;

        new_symbol->constant_type = get_singleton("bool");
        return semantic_type_bool;
    } else if (leaf->symbol->str == get_singleton("null")) {
        // null
        symbol_t* new_symbol = get_symbol(symbol_type_constant, leaf->symbol->str);

        new_symbol->line_number = leaf->symbol->line_number;
        leaf->symbol = new_symbol;

        new_symbol->constant_type = get_singleton("null");
        return semantic_type_null;
    } else {
        // string or number
        if (leaf->symbol->str->strval[0] == '"' && leaf->symbol->str->strval[strlen(leaf->symbol->str->strval) - 1] == '"') {
            // string
            symbol_t* new_symbol = get_symbol(symbol_type_constant, leaf->symbol->str);

            new_symbol->line_number = leaf->symbol->line_number;
            leaf->symbol = new_symbol;

            new_symbol->constant_type = get_singleton("string");
            return semantic_type_string;
        } else {
            // number
            int float_flag = 0;
            for (size_t i = 0; i < strlen(leaf->symbol->str->strval); i++) {
                if (i > 0) {
                    if (leaf->symbol->str->strval[i] == '.' || leaf->symbol->str->strval[i] == 'e' || leaf->symbol->str->strval[i] == 'E') {
                        float_flag = 1;
                    }
                }
            }
            if (float_flag) {
                // float
                symbol_t* new_symbol = get_symbol(symbol_type_constant, leaf->symbol->str);

                new_symbol->line_number = leaf->symbol->line_number;
                leaf->symbol = new_symbol;
                
                new_symbol->constant_type = get_singleton("float");
                new_symbol->constant_value_float = atof(leaf->symbol->str->strval);
                return semantic_type_float;
            } else {
                // int
                symbol_t* new_symbol = get_symbol(symbol_type_constant, leaf->symbol->str);

                new_symbol->line_number = leaf->symbol->line_number;
                leaf->symbol = new_symbol;

                new_symbol->constant_type = get_singleton("int");
                new_symbol->constant_value_int = atoi(leaf->symbol->str->strval);
                return semantic_type_int;
            }
        }
    }
    return semantic_type_dynamic;
}

ast_function_t* semantic_check_id(ast_leaf_t* leaf, ast_function_list_t* function_list)
{
    if (function_list->next == NULL) {
        varstring_t* error_msg = varstring_init();
        varstring_write(error_msg, "function not declared ");
        formatter_state_t state = { 0 };
        formatter_print_leaf(leaf, &state, error_msg->stream);
        varstring_write(error_msg, " on line %d", leaf->symbol->line_number);
        throw_error(3, "%s", varstring_destroy(error_msg)->strval);
    }
    ast_function_list_t* current_function = function_list->next;

    while (current_function != NULL) {
        if (leaf->symbol == current_function->item->name) {
            return current_function->item;
        }
        current_function = current_function->next;
    }
    varstring_t* error_msg = varstring_init();
    varstring_write(error_msg, "function not declared ");
    formatter_state_t state = { 0 };
    formatter_print_leaf(leaf, &state, error_msg->stream);
    varstring_write(error_msg, " on line %d", leaf->symbol->line_number);
    throw_error(3, "%s", varstring_destroy(error_msg)->strval);
    return NULL;
}

semantic_type_t semantic_return_type(ast_function_t* function)
{

    if (function->returned_type == get_symbol_by_str(symbol_type_keyword, "int")) {
        return semantic_type_int;
    } else if (function->returned_type == get_symbol_by_str(symbol_type_keyword, "float")) {
        return semantic_type_float;
    } else if (function->returned_type == get_symbol_by_str(symbol_type_keyword, "string")) {
        return semantic_type_string;
    } else if (function->returned_type == get_symbol_by_str(symbol_type_keyword, "bool")) {
        return semantic_type_bool;
    } else if (function->returned_type == get_symbol_by_str(symbol_type_keyword, "void")) {
        return semantic_type_dynamic;
    }
    return semantic_type_dynamic;
}

semantic_type_t semantic_check_expression(ast_node_t* item, ast_function_list_t* function_list)
{
    if (item->leaf != NULL) {
        // semantic_check_leaf
        if (item->leaf->symbol->type == symbol_type_local_variable) {
            // variable
            return semantic_type_dynamic;
        } else if (item->leaf->symbol->str->strval[0] == '$') {
            // unmarked variable
            // this cannot happen
            throw_error(99, "unmarked variable %s on line %d", item->leaf->symbol->str->strval, item->leaf->symbol->line_number);
        } else if (item->leaf->symbol->type == symbol_type_function_identifier) {
            // function call
            if ((item->leaf->symbol->fn == NULL)) {
                // buid-in function
                size_t parameter_count = item->leaf->call_parameters->size;
                if (item->leaf->symbol != get_symbol_by_str(symbol_type_function_identifier, "write")) {
                    if (item->leaf->call_parameters->size != semantic_buildin_parameter_count(item->leaf)){
                        // function has wrong number of parameters
                        varstring_t* error_msg = varstring_init();
                        varstring_write(error_msg, "wrong number of parameters in call of function ");
                        formatter_state_t state = { 0 };
                        formatter_print_leaf(item->leaf, &state, error_msg->stream);
                        varstring_write(error_msg, " on line %d", semantic_get_line_number(item));
                        throw_error(4, "%s", varstring_destroy(error_msg)->strval);
                    }
                }

                for (size_t i = 0; i < parameter_count; i++){
                    semantic_check_expression(item->leaf->call_parameters->parameters[i]->node, function_list);
                }
                return semantic_buildin_return_type(item->leaf);
            }
            ast_function_t* function = semantic_check_id(item->leaf, function_list);
            // semantic_check_call_parameters

            if (function->parameters->count != item->leaf->call_parameters->size) {
                varstring_t* error_msg = varstring_init();
                varstring_write(error_msg, "wrong number of parameters in call of function ");
                formatter_state_t state = { 0 };
                formatter_print_leaf(item->leaf, &state, error_msg->stream);
                varstring_write(error_msg, " on line %d", item->leaf->symbol->line_number);
                throw_error(4, "%s", varstring_destroy(error_msg)->strval);
            }

            symbol_t* string_symbol = get_symbol_by_str(symbol_type_keyword, "string");
            symbol_t* bool_symbol = get_symbol_by_str(symbol_type_keyword, "bool");

            for (size_t i = 0; i < item->leaf->call_parameters->size; i++) {
                ast_call_parameter_t* current_parameter = item->leaf->call_parameters->parameters[i];
                semantic_type_t parameter_type = semantic_check_expression(current_parameter->node, function_list);
                ast_parameter_t* function_parameter = function->parameters->parameters[i];

                if (parameter_type != semantic_type_dynamic) {
                    if (!function_parameter->optional && parameter_type != semantic_type_null) {
                        if (parameter_type == semantic_type_int) {
                            if (function_parameter->type == string_symbol) {
                                varstring_t* error_msg = varstring_init();
                                varstring_write(error_msg, "int parameter type given instead of %s ", function_parameter->type->str->strval);
                                formatter_state_t state = { 0 };
                                formatter_print_expression(item, &state, error_msg->stream);
                                varstring_write(error_msg, " on line %d", current_parameter->node->leaf->symbol->line_number);
                                throw_error(4, "%s", varstring_destroy(error_msg)->strval);
                            }
                        } else if (parameter_type == semantic_type_float) {
                            if (function_parameter->type == string_symbol) {
                                varstring_t* error_msg = varstring_init();
                                varstring_write(error_msg, "float parameter type given instead of %s ", function_parameter->type->str->strval);
                                formatter_state_t state = { 0 };
                                formatter_print_expression(item, &state, error_msg->stream);
                                varstring_write(error_msg, " on line %d", current_parameter->node->leaf->symbol->line_number);
                                throw_error(4, "%s", varstring_destroy(error_msg)->strval);
                            }
                        } else if (parameter_type == semantic_type_string) {
                            if (function_parameter->type != string_symbol || function_parameter->type != bool_symbol) {
                                varstring_t* error_msg = varstring_init();
                                varstring_write(error_msg, "string parameter type given instead of %s type ", function_parameter->type->str->strval);
                                formatter_state_t state = { 0 };
                                formatter_print_expression(item, &state, error_msg->stream);
                                varstring_write(error_msg, " on line %d", current_parameter->node->leaf->symbol->line_number);
                                throw_error(4, "%s", varstring_destroy(error_msg)->strval);
                            }
                        }
                    }
                }
            }

            return semantic_return_type(function);
        } else {
            // constant
            semantic_type_t const_type = semantic_constant_type(item->leaf);
            if (const_type == semantic_type_dynamic) {
                varstring_t* error_msg = varstring_init();
                varstring_write(error_msg, "constant type expected instead of ");
                formatter_state_t state = { 0 };
                formatter_print_expression(item, &state, error_msg->stream);
                varstring_write(error_msg, " on line %d", semantic_get_line_number(item));
                throw_error(4, "%s", varstring_destroy(error_msg)->strval);
            }
            return const_type;
        }
        return semantic_type_dynamic;
    }
    if (item->op != NULL) {
        semantic_type_t left_type = semantic_check_expression(item->left, function_list);
        semantic_type_t right_type = semantic_check_expression(item->right, function_list);

        if (left_type == right_type) {
            return left_type;
        } else if (left_type == semantic_type_dynamic) {
            return right_type;
        } else if (right_type == semantic_type_dynamic) {
            return left_type;
        } else if (left_type == semantic_type_string || right_type == semantic_type_string) {
            if (left_type == semantic_type_dynamic && right_type == semantic_type_string) {
                return semantic_type_string;
            } else {
                varstring_t* error_msg = varstring_init();
                varstring_write(error_msg, "cannot operate string with non-string  ");
                formatter_state_t state = { 0 };
                formatter_print_expression(item, &state, error_msg->stream);
                varstring_write(error_msg, " on line %d", semantic_get_line_number(item));
                throw_error(7, "%s", varstring_destroy(error_msg)->strval);
            }
        } else if (left_type == semantic_type_int && right_type == semantic_type_float) {
            return semantic_type_float;
        } else if (left_type == semantic_type_float && right_type == semantic_type_int) {
            return semantic_type_float;
        } else if (left_type == semantic_type_bool && right_type != semantic_type_bool) {
            return right_type;
        } else if (left_type != semantic_type_bool && right_type == semantic_type_bool) {
            return left_type;
        } else {
            return semantic_type_dynamic;
        }
    }
    return semantic_type_dynamic;
}

void semantic_check_block(ast_block_t* block, ast_function_list_t* function_list)
{
    ast_block_item_t* current_item = block->first;
    for (size_t i = 0; i < block->num_items; i++) {
        if (current_item->item != NULL) {
            semantic_check_expression(current_item->item, function_list);
        } else if (current_item->conditional != NULL) {
            semantic_check_expression(current_item->conditional->condition, function_list);
            semantic_check_block(current_item->conditional->true_branch, function_list);
            semantic_check_block(current_item->conditional->false_branch, function_list);
        } else if (current_item->loop != NULL) {
            if (current_item->loop->condition != NULL)
                semantic_check_expression(current_item->loop->condition, function_list);
            if (current_item->loop->initializer != NULL)
                semantic_check_expression(current_item->loop->initializer, function_list);
            if (current_item->loop->incrementer != NULL)
                semantic_check_expression(current_item->loop->incrementer, function_list);
            semantic_check_block(current_item->loop->body, function_list);
        }

        current_item = current_item->next;
    }
}

// get_symbol_by_str(symbol_type_keyword, "int");

void semantic_check(ast_function_list_t* function_list)
{
    ast_function_list_t* main_function = function_list;
    ast_function_list_t* current_function = function_list->next;

    while (current_function != NULL) {
        semantic_check_block(current_function->item->block, function_list);
        current_function = current_function->next;
    }
    semantic_check_block(main_function->item->block, function_list);
}

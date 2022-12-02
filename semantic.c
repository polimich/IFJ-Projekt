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

semantic_type_t semantic_constant_type(ast_leaf_t* leaf)
{
    // TODO return type of constant
}

ast_function_t* semantic_check_id(ast_leaf_t* leaf, ast_function_list_t* function_list)
{
    // TODO build-in functions
    if (function_list->next == NULL) {
        throw_error(99, "semantic: Function not declared");
    }
    ast_function_list_t* current_function = function_list->next;

    while (current_function != NULL) {
        if (leaf->symbol == current_function->item->name) {
            return current_function->item;
        }
        current_function = current_function->next;
    }

    throw_error(99, "semantic: Function not declared");
    return NULL;
}

semantic_type_t semantic_return_type(ast_function_t* function)
{
    // TODO build-in functions

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
    } else {
        throw_error(99, "semantic: Function return type not defined");
    }
    return semantic_type_dynamic;
}

semantic_type_t semantic_check_expression(ast_node_t* item, ast_function_list_t* function_list)
{
    if (item->leaf != NULL) {
        // semantic_check_leaf
        if (item->leaf->symbol->type == symbol_type_local_variable) {
            return semantic_type_dynamic;
        } else if (item->leaf->symbol->type == symbol_type_function_identifier) {
            ast_function_t* function = semantic_check_id(item->leaf, function_list);

            // TODO optional parameters
            //semantic_check_call_parameters
            for (size_t i = 0; i < item->leaf->call_parameters->size; i++) {
                ast_call_parameter_t* current_parameter = item->leaf->call_parameters->parameters[i];
                semantic_type_t parameter_type = semantic_check_expression(current_parameter->node, function_list);

                if (parameter_type != semantic_type_dynamic) {
                    if (parameter_type == semantic_type_int) {
                        if (function->parameters->parameters[i]->type != get_symbol_by_str(symbol_type_keyword, "int")) {
                            throw_error(99, "semantic: wrong parameter type");
                        }
                    } else if (parameter_type == semantic_type_float) {
                        if (function->parameters->parameters[i]->type != get_symbol_by_str(symbol_type_keyword, "float")) {
                            throw_error(99, "semantic: wrong parameter type");
                        }
                    } else if (parameter_type == semantic_type_string) {
                        if (function->parameters->parameters[i]->type != get_symbol_by_str(symbol_type_keyword, "string")) {
                            throw_error(99, "semantic: wrong parameter type");
                        }
                    } else if (parameter_type == semantic_type_bool) {
                        if (function->parameters->parameters[i]->type != get_symbol_by_str(symbol_type_keyword, "bool")) {
                            throw_error(99, "semantic: wrong parameter type");
                        }
                    }
                } else {
                    return semantic_type_dynamic;
                }
            }

            return semantic_return_type(function);
        } else {
            // TODO constants
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
                throw_error(99, "semantic: Cannot operate on string and non-string");
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
            // FIXME bude fungovat pouze pro while
            semantic_check_expression(current_item->loop->condition, function_list);
            semantic_check_block(current_item->loop->body, function_list);
        } else {
            throw_warning(99, "semantic: empty block item");
        }
        current_item = current_item->next;
    }
}

// get_symbol_by_str(symbol_type_keyword, "int");

void semantic_check_function(ast_function_t* function, ast_function_list_t* function_list)
{

    if (function->block == NULL) {
        // destoy function, function does nothing
        return;
    }
    semantic_check_block(function->block, function_list);
}

void semantic_init(ast_function_list_t* function_list)
{
    // TODO kontrolu mainu az po nacteni ostatnich funkci
    ast_function_list_t* current_function = function_list->next;

    while (current_function != NULL) {
        semantic_check_function(current_function->item, function_list);
        current_function = current_function->next;
    }
}

// symbol types clasified from parser:
// operator
// function identifier
// variable identifier

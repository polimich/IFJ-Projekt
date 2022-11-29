//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace ast.h: xpoliv07, xmedri01            //
//    Datum: 26. 11. 2022 - x. 11. 2022                 // TODO
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./generator.h"
#include "./error.h"

void generator_print_symbol(symbol_t* symbol, FILE* output)
{
    fprintf(output, "%s", symbol->str->strval);
    // TODO generate symbol
}
void generator_print_leaf(ast_leaf_t* leaf, FILE* output)
{

    generator_print_symbol(leaf->symbol, output);
    if (leaf->call_parameters) {
        generator_print_call_parameter_list(leaf->call_parameters, output);
    }
}
void generator_print_statement(ast_node_t* node, FILE* output)
{
    if (node->leaf) {
        generator_print_leaf(node->leaf, output);
    } else {
        generator_print_symbol(node->op, output);
        generator_print_statement(node->left, output);
        generator_print_statement(node->right, output);
    }
}

void generator_print_conditional(ast_conditional_t* conditional, FILE* output)
{
    // TODO generate IF

    generator_print_block(conditional->true_branch, output);
    if (conditional->false_branch) {
        // TODO generate ELSE
        generator_print_block(conditional->false_branch, output);
    }
}

void generator_print_loop(ast_loop_t* loop, FILE* output)
{
    // TODO loop print
    if (loop->initializer) {

    } else if (loop->condition) {

    } else if (loop->incrementer) {
    }
    generator_print_block(loop->body, output);
}

void generator_print_block_item(ast_block_item_t* block_item, FILE* output)
{
    if (block_item->item) {
        generator_print_statement(block_item->item, output);
    } else if (block_item->conditional) {
        generator_print_conditional(block_item->conditional, output);
    } else if (block_item->loop) {
        generator_print_loop(block_item->loop, output);

    } else {
        throw_warning(99, "Empty block item");
    }
}

void generator_print_block(ast_block_t* block, FILE* output)
{
    for (ast_block_item_t* item = block->first; item != NULL; item = item->next) {
        generator_print_block_item(item, output);
    }
}

void generator_print_call_parameter(ast_call_parameter_t* parameter, FILE* output)
{
    // TODO - call parameter - add to stack
}

void generator_print_call_parameter_list(ast_call_parameter_list_t* call_parameter_list, FILE* output)
{
    for (size_t i = 0; i < call_parameter_list->size; ++i) {
        generator_print_call_parameter(call_parameter_list->parameters[i], output);
    }
}

void generator_print_parameter(ast_parameter_t* parameter, FILE* output)
{
    if (parameter->optional) {
        // TODO generate optional parameter
    }
    // TODO check type before print
    generator_print_symbol(parameter->type, output);
    generator_print_symbol(parameter->name, output);
}

void generator_print_parameter_list(ast_parameter_list_t* parameter_list, FILE* output)
{
    for (size_t i = 0; i < parameter_list->count; i++) {
        generator_print_parameter(parameter_list->parameters[i], output);
    }
    return;
}

void generator_print_function(ast_function_t* function, FILE* output)
{
    // TODO generate label for function
    generator_print_parameter_list(function->parameters, output);
    generator_print_block(function->block, output);
    // TODO generate retval
}

void generator_print_function_list(ast_function_list_t* function_list, FILE* output)
{
    generator_print_function(function_list->item, output);
    putc("\n", output);
    if (function_list->next) {
        generator_print_function_list(function_list->next, output);
    }
}

void generator(ast_function_list_t* function_list, FILE* output)
{
    // check if parser returned ast
    if (function_list == NULL) {
        throw_error(2, "AST is empty");
    }
    // main function stored for generating later
    ast_function_t* main = function_list->item;

    fprintf(output, ".IFJcode22\nJUMP $main");
    // generate code for all functions
    if (function_list->next) {
        generator_print_function_list(function_list->next, output);
    }
    generator_print_block(main->block, output);
}

/*
void generate_block(ast_block_t* block)
{
    ast_block_item_t* statement = block->first;
    while (statement != NULL) {
        if (statement->item != NULL) {
            generate_node(statement->item);
        } else if (statement->conditional != NULL) {
            generate_conditional(statement->conditional);
        } else if (statement->loop != NULL) {
            generate_loop(statement->loop);
        } else {
            throw_warning(10, "function empty should not happen");
        }
        statement = statement->next;
    }
    return;
}

void generate_node(ast_node_t* node)
{

    if (node->op->type == symbol_type_operator) {
        if (node->op->str == get_singleton("+")) {
            generate_node(node->left);
            generate_node(node->right);
            // TODO print code add

        } else if (node->op->str == get_singleton("-")) {
            generate_node(node->left);
            generate_node(node->right);
            // TODO print code sub
        } else if (node->op->str == get_singleton("*")) {
            generate_node(node->left);
            generate_node(node->right);
            // TODO print code mul
        } else if (node->op->str == get_singleton("/")) {
            generate_node(node->left);
            generate_node(node->right);
            // TODO print code div
        } else if (node->op->str == get_singleton("<")) {
            generate_node(node->left);
            generate_node(node->right);
            // TODO print code less
        } else if (node->op->str == get_singleton(">")) {
            generate_node(node->left);
            generate_node(node->right);
            // TODO print code greater
        } else if (node->op->str == get_singleton("<=")) {
            generate_node(node->left);
            generate_node(node->right);
            // TODO print code less or equal
        } else if (node->op->str == get_singleton(">=")) {
            generate_node(node->left);
            generate_node(node->right);
            // TODO print code greater or equal

        } else if (node->op->str == get_singleton("==")) {
            generate_node(node->left);
            generate_node(node->right);
            // TODO print code equals
        } else if (node->op->str == get_singleton("!=")) {
            generate_node(node->left);
            generate_node(node->right);
            // TODO print code not equals
        } else if (node->op->str == get_singleton("=")) {
            // TODO print code assign
        } else {
            throw_warning(10, "unknown operator, should not happen");
        }
        return;
    } else if (node->op == NULL) {
        if (node->leaf->symbol->type == symbol_type_constant) {
            // TODO print code constant
        } else if (node->leaf->symbol->type == symbol_type_local_variable) {
            // TODO print code local variable
        } else {
            throw_warning(10, "unknown symbol, should not happen");
        }
    }
}


*/
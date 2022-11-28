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

#include "./code_generator.h"

void interpreter(ast_function_list_t* function_list)
{
    // check if parser returned ast
    if (function_list == NULL) {
        throw_error(2, "AST is empty");
    }
    // main function stored for generating later
    ast_function_t* main = function_list->item;

    generate_header();
    // generate code for all functions
    while (function_list->next != NULL) {
        generate_function(function_list->next->item);
        function_list = function_list->next;
    }
    generate_function(main);
    return;
}

void generate_function(ast_function_t* function)
{
    const char* name = function->name->str->strval;
    singleton_t* label = generate_label(function->name->str);
    generate_code_label(label); // TODO label
    generate_parameters(function->parameters);
    generate_block(function->block);
    return;
}

void generate_parameters(ast_parameter_list_t* parameters)
{
    for (int i = 0; i < parameters->count; i++) {
        ast_parameter_t* parameter = parameters->parameters[i];
        // TODO generate parameter
    }
    return;
}

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

void generate_header()
{
    fprintf(stdout, ".IFJcode22");
    fprintf(stdout, "JUMP $main\n");
    return;
}

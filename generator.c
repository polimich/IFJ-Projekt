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

void generator_print_assignment(ast_node_t* node, __GEN_DREST__)
{
    generator_print_expression(node->right, __GEN_CREST__);
    fprintf(output, "POPS LF@%s\n", node->left->leaf->symbol->str->strval);
    return;
}
void generator_print_return(ast_node_t* node, __GEN_DREST__)
{
    (void)data;
    (void)output;
    (void)node;
}
void generator_print_operator(symbol_t* op, __GEN_DREST__)
{
    if (op->str == get_singleton("+")) {
        fprintf(output, "CALL $ADD\n");
        data->add_generated = true;
    } else if (op->str == get_singleton("-")) {
        fprintf(output, "CALL $SUB\n");
        data->sub_generated = true;
    } else if (op->str == get_singleton("*")) {
        fprintf(output, "CALL $MUL\n");
        data->mul_generated = true;
    } else if (op->str == get_singleton("/")) {
        fprintf(output, "CALL $ADD\n");
        data->div_generated = true;
    } else if (op->str == get_singleton("<")) {
        fprintf(output, "CALL $LT\n");
    } else if (op->str == get_singleton("<=")) {
        fprintf(output, "CALL $LTEQ\n");
    } else if (op->str == get_singleton(">")) {
        fprintf(output, "CALL $GT\n");
    } else if (op->str == get_singleton(">=")) {
        fprintf(output, "CALL $GTEQ\n");
    } else if (op->str == get_singleton("===")) {
        fprintf(output, "CALL $EQ\n");
    } else if (op->str == get_singleton("!=")) {
        fprintf(output, "CALL $NEQ\n");
    }
    (void)data;
}

void generator_print_call_parameters(ast_call_parameter_list_t* list, __GEN_DREST__)
{
    (void)data;
    (void)output;
    (void)list;
}

void generator_print_constant(symbol_t* constant, __GEN_DREST__)
{
    (void)data;
    fprintf(output, "%s\n", constant->constant_type->strval);

    if (constant->constant_type == get_singleton("int")) {
        fprintf(output, "PUSHS int@%d\n", constant->constant_value_int);
    } else if (constant->constant_type == get_singleton("float")) {
        fprintf(output, "PUSHS float@%a\n", constant->constant_value_float);
    } else if (constant->constant_type == get_singleton("bool")) {
        fprintf(output, "PUSHS bool@%s\n", constant->str->strval);
    } else if (constant->constant_type == get_singleton("string")) {
        fprintf(output, "PUSHS %s\n", encoder_encode_string(decoder_decode_string(constant->str))->strval);
    } else if (constant->constant_type == get_singleton("nil")) {
        fprintf(output, "PUSHS nil@nil\n");
    }
}

void generator_print_local_variable(symbol_t* local_variable, __GEN_DREST__)
{
    (void)data;
    // TODO if value not declared -> error
    fprintf(output, "PUSHS LF@%s\n", local_variable->str->strval);
}
void generator_print_function_call(ast_leaf_t* function, __GEN_DREST__)
{
    if (function->call_parameters) {
        for (size_t i = 0; i < function->call_parameters->size; i++) {
            symbol_t* parameter = function->call_parameters->parameters[i]->node->leaf->symbol;
            if (parameter->type == symbol_type_constant) {
                generator_print_constant(parameter, __GEN_CREST__);
            } else if (parameter->type == symbol_type_local_variable) {
                fprintf(output, "call parameter declared=%d\n", parameter->variable_is_declared); // TODO remove
                if (parameter->variable_is_declared) {
                    generator_print_local_variable(parameter, __GEN_CREST__);
                } else {
                    throw_warning(4, "parameter %s has no value or is not declared", parameter->str->strval);
                }
            }
        }
    }
    fprintf(output, "CALL %s\n", generate_label(function->symbol)->strval);
}
void generator_print_expression(ast_node_t* node, __GEN_DREST__)
{
    if (node->leaf) {
        // when there is constant or var in expression push to stack
        if (node->leaf->symbol->type == symbol_type_constant) {
            generator_print_constant(node->leaf->symbol, __GEN_CREST__);
        } else if (node->leaf->symbol->type == symbol_type_local_variable || true) {
            generator_print_local_variable(node->leaf->symbol, __GEN_CREST__);
        }
    } else {
        generator_print_expression(node->left, __GEN_CREST__);
        generator_print_expression(node->right, __GEN_CREST__);
        generator_print_operator(node->op, __GEN_CREST__);
    }
}

void generator_print_statement(ast_node_t* node, __GEN_DREST__)
{
    // assignment
    if (node->leaf) {
        if (node->leaf->call_parameters) {
            generator_print_function_call(node->leaf, __GEN_CREST__);
        }
    } else {
        if (node->op->str == get_singleton("="))
            generator_print_assignment(node, __GEN_CREST__);
        // expresion
        else if (node->op->str != NULL) {
            generator_print_expression(node, __GEN_CREST__);
        }
    }
}

void generator_print_conditional(ast_conditional_t* conditional, __GEN_DREST__)
{
    generator_print_expression(conditional->condition, __GEN_CREST__);
    fprintf(output, "PUSHS bool@true\n");
    fprintf(output, "JUMPIFNEQS $ELSE%s\n", generate_label(conditional->condition)->strval);
    generator_print_block(conditional->true_branch, __GEN_CREST__);
    if (conditional->false_branch) {
        fprintf(output, "JUMP ENDIF$%s\n", generate_label(conditional->condition)->strval);
        fprintf(output, "LABEL $ELSE%s\n", generate_label(conditional->condition)->strval);
        generator_print_block(conditional->false_branch, __GEN_CREST__);
    }
    fprintf(output, "LABEL $ENDIF%s\n", generate_label(conditional->condition)->strval);
}

void generator_print_loop(ast_loop_t* loop, __GEN_DREST__)
{
    if (loop->initializer) {
        generator_print_statement(loop->initializer, __GEN_CREST__);
    }
    fprintf(output, "LABEL $WHILE$%s \n", generate_label(loop)->strval);

    if (loop->condition) {
        generator_print_expression(loop->condition, __GEN_CREST__);
        fprintf(output, "PUSHS bool@true\n");
        fprintf(output, "JUMPIFNEQS $WHILE$END %s bool@true\n", generate_label(loop)->strval);
    }
    generator_print_block(loop->body, __GEN_CREST__);
    if (loop->incrementer) {
        generator_print_statement(loop->incrementer, __GEN_CREST__);
    }

    fprintf(output, "JUMP $WHILE$%s\n", generate_label(loop)->strval);
    fprintf(output, "LABEL $WHILE_END$%s\n", generate_label(loop)->strval);
}

void generator_print_block_item(ast_block_item_t* block_item, __GEN_DREST__)
{

    if (block_item->item) {
        generator_print_statement(block_item->item, __GEN_CREST__);
    } else if (block_item->conditional) {
        generator_print_conditional(block_item->conditional, __GEN_CREST__);
    } else if (block_item->loop) {
        generator_print_loop(block_item->loop, __GEN_CREST__);
    } else if (block_item->is_return_statement) {
        generator_print_return(block_item->item, __GEN_CREST__);
    } else {
        // should not happen
        throw_warning(99, "Empty block item\n");
    }
}

void generator_print_block(ast_block_t* block, __GEN_DREST__)
{
    for (ast_block_item_t* item = block->first; item != NULL; item = item->next) {
        generator_print_block_item(item, __GEN_CREST__);
    }
}

void generator_print_call_parameter(ast_call_parameter_t* parameter, __GEN_DREST__)
{
    (void)data;
    (void)output;
    (void)parameter;
    // TODO - call parameter - add to stack
}

void generator_print_call_parameter_list(ast_call_parameter_list_t* call_parameter_list, __GEN_DREST__)
{
    for (size_t i = 0; i < call_parameter_list->size; ++i) {
        generator_print_call_parameter(call_parameter_list->parameters[i], __GEN_CREST__);
    }
}

void generator_print_parameter(ast_parameter_t* parameter, __GEN_DREST__)
{
    (void)data;
    (void)output;
    if (parameter->optional) {
        // TODO generate optional parameter
    }
    // TODO check type before print
    // generator_print_symbol(parameter->type, __GEN_CREST__);
    // generator_print_symbol(parameter->name, __GEN_CREST__);
}

void generator_print_parameter_list(ast_function_t* function, __GEN_DREST__)
{
    (void)data;
    (void)output;
    (void)function;
}
void generator_print_variable_declarations(FILE* output, symtable_t* symtable)
{
    if (symtable->lnode) {
        generator_print_variable_declarations(output, symtable->lnode);
    }
    if (symtable->rnode) {
        generator_print_variable_declarations(output, symtable->rnode);
    }
    if (symtable->symbol->type == symbol_type_local_variable)
        fprintf(output, "DEFVAR LF@%s\n", symtable->symbol->str->strval);
}

void generator_print_function(ast_function_t* function, __GEN_DREST__)
{
    (void)data;
    // TODO generate label for function
    singleton_t* label = generate_label(function);
    fprintf(output, "LABEL $%s\n", label->strval);

    generator_print_variable_declarations(output, function->symtable);

    generator_print_parameter_list(function, __GEN_CREST__);
    //  generator_print_block(function->block, output);
    //   TODO generate retval
    generator_print_block(function->block, __GEN_CREST__);
    fprintf(output, "RETURN\n");
}
void generator_print_main(ast_function_t* function, __GEN_DREST__)
{
    (void)data;

    fprintf(output, "LABEL $$MAIN\n");
    fprintf(output, "CREATEFRAME\n");
    fprintf(output, "PUSHFRAME\n");
    // define
    generator_print_variable_declarations(output, function->symtable);
    generator_print_parameter_list(function, __GEN_CREST__);
    generator_print_block(function->block, __GEN_CREST__);
    fprintf(output, "POPFRAME\n");
    fprintf(output, "EXIT int@0\n");
}

void generator_print_function_list(ast_function_list_t* function_list, __GEN_DREST__)
{
    (void)data;
    generator_print_function(function_list->item, __GEN_CREST__);

    putc('\n', output);
    if (function_list->next) {
        generator_print_function_list(function_list->next, __GEN_CREST__);
    }
}

void generator_print_built_in(__GEN_DREST__)
{
    (void)data;
    if (data->add_generated) {
        fprintf(output, GENERATOR_ADD);
    }
    if (data->sub_generated) {
        fprintf(output, GENERATOR_SUB);
    }
    if (data->mul_generated) {
        fprintf(output, GENERATOR_MUL);
    }
    if (data->div_generated) {
        fprintf(output, GENERATOR_DIV);
    }
}

void generator(ast_function_list_t* function_list, FILE* output)
{

    generator_data_t data = {
        .add_generated = false,
        .sub_generated = false,
        .div_generated = false,
        .mul_generated = false,
    };
    // check if parser returned ast
    if (function_list == NULL) {
        throw_error(2, "AST is empty");
    }
    // main function stored for generating later
    ast_function_t* main = function_list->item;

    fprintf(output, ".IFJcode22\nJUMP $$main\n");
    // generate code for all functions
    if (function_list->next) {
        generator_print_function_list(function_list->next, &data, output);
    }
    generator_print_main(main, &data, output);
    generator_print_built_in(&data, output);
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
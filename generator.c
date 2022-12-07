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
    node->left->leaf->symbol->variable_is_declared = true;
    node->left->leaf->symbol->variable_has_value = true;
    return;
}
void generator_print_return(ast_node_t* node, __GEN_DREST__)
{
    generator_print_expression(node, __GEN_CREST__);
    ast_function_t* function = data->function;
    if (function->returned_type) {
        if (function->returned_type->str == get_singleton("int"))
            fprintf(output, "PUSHS int@0\n");
        else if (function->returned_type->str == get_singleton("float"))
            fprintf(output, "PUSHS float@0x0.0p+0\n");
        else if (function->returned_type->str == get_singleton("bool"))
            fprintf(output, "PUSHS bool@false\n");
        else if (function->returned_type->str == get_singleton("string"))
            fprintf(output, "PUSHS string@\n");
        fprintf(output, "PUSHS LF@retval\n");
        fprintf(output, "CALL $TO_GOOD_TYPE\n");
        fprintf(output, "POPS LF@retval\n");
        fprintf(output, "POPS LF@retval\n");
        /*
        if (returnedtype == optional)
            if (retval_nul)
                TYPE LF @retval$type LF @retval if LF @retval$type == null

                        fprintf(output, "JUMPIFEQ \n");*/
        fprintf(output, "CALL $TO_GOOD_TYPE\n");
        fprintf(output, "POPS LF@retval\n");
        fprintf(output, "PUSHS LF@retval\n");
    }

    fprintf(output, "RETURN\n");
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
        fprintf(output, "CALL $DIV\n");
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
    } else if (op->str == get_singleton(".")) {
        fprintf(output, "CALL $CONCAT\n");
    }
    (void)data;
}

void generator_print_constant(symbol_t* constant, __GEN_DREST__)
{
    (void)data;

    if (constant->constant_type == get_singleton("int")) {
        fprintf(output, "PUSHS int@%d\n", constant->constant_value_int);
    } else if (constant->constant_type == get_singleton("float")) {
        fprintf(output, "PUSHS float@%a\n", constant->constant_value_float);
    } else if (constant->constant_type == get_singleton("bool")) {
        fprintf(output, "PUSHS bool@%s\n", constant->str->strval);
    } else if (constant->constant_type == get_singleton("string")) {
        fprintf(output, "PUSHS %s\n", encoder_encode_string(decoder_decode_string(constant->str))->strval);
    } else if (constant->constant_type == get_singleton("null")) {
        fprintf(output, "PUSHS nil@nil\n");
    }
}

void generator_print_local_variable(symbol_t* local_variable, __GEN_DREST__)
{
    (void)data;
    if (local_variable->variable_is_declared)
        fprintf(output, "PUSHS LF@%s\n", local_variable->str->strval);
}
void generator_print_function_call(ast_leaf_t* function, __GEN_DREST__)
{

    if (function->call_parameters) {

        for (int i = function->call_parameters->size - 1; i >= 0; i--) {
            ast_node_t* parameter = function->call_parameters->parameters[i]->node;
            generator_print_expression(parameter, __GEN_CREST__);
        }
    }

    if (function->symbol->fn == NULL) {
        if (function->symbol->str == get_singleton("write")) {
            for (size_t i = 0; i < function->call_parameters->size; i++)
                fprintf(output, "CALL $WRITE\n");
        } else if (function->symbol->str == get_singleton("reads")) {
            fprintf(output, "CALL $READS\n");
        } else if (function->symbol->str == get_singleton("readi")) {
            fprintf(output, "CALL $READI\n");
        } else if (function->symbol->str == get_singleton("readf")) {
            fprintf(output, "CALL $READF\n");
        } else if (function->symbol->str == get_singleton("floatval")) {
            fprintf(output, "CALL $floatval\n");
        } else if (function->symbol->str == get_singleton("intval")) {
            fprintf(output, "CALL $intval\n");
        } else if (function->symbol->str == get_singleton("strval")) {
            fprintf(output, "CALL $strval\n");
        } else if (function->symbol->str == get_singleton("strlen")) {
            fprintf(output, "CALL $STRLEN\n");
        } else if (function->symbol->str == get_singleton("substr")) {
            fprintf(output, "CALL $SUBSTRING\n");
        } else if (function->symbol->str == get_singleton("chr")) {
            fprintf(output, "CALL $CHR\n");
        } else if (function->symbol->str == get_singleton("ord")) {
            fprintf(output, "CALL $ORD\n");
        } else
            fprintf(output, "CALL $FUNCTION$%s\n", generate_label(function->symbol)->strval);
    }
}
void generator_print_expression(ast_node_t* node, __GEN_DREST__)
{
    if (node->leaf) {
        if (node->leaf->call_parameters) {
            generator_print_function_call(node->leaf, __GEN_CREST__);
        }
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

void generator_print_parameter_list(ast_function_t* function, __GEN_DREST__)
{
    (void)data;
    (void)output;

    for (size_t i = 0; i < function->parameters->count; i++) {
        ast_parameter_t* param = function->parameters->parameters[i];
        const char* func_name = generate_label(function->name)->strval;
        fprintf(output, "POPS LF@%s\n", param->name->str->strval);
        fprintf(output, "DEFVAR LF@%s$type\n", param->name->str->strval);
        fprintf(output, "DEFVAR LF@%s$optional\n", param->name->str->strval);
        fprintf(output, "DEFVAR LF@%s$expected_type\n", param->name->str->strval);
        fprintf(output, "MOVE LF@%s$optional bool@%s\n", param->name->str->strval, param->optional ? "true" : "false");
        if (param->type->str == get_singleton("string"))
            fprintf(output, "MOVE LF@%s$expected_type string@string\n", param->name->str->strval);
        else if (param->type->str == get_singleton("int"))
            fprintf(output, "MOVE LF@%s$expected_type string@int\n", param->name->str->strval);
        else if (param->type->str == get_singleton("float"))
            fprintf(output, "MOVE LF@%s$expected_type string@float\n", param->name->str->strval);
        else if (param->type->str == get_singleton("bool"))
            fprintf(output, "MOVE LF@%s$expected_type string@bool\n", param->name->str->strval);
        fprintf(output, "TYPE LF@%s$type LF@$n\n", param->name->str->strval);
        fprintf(output, "JUMPIFNEQ $WRONG_TYPE$%s LF@%s$type LF@%s$expected_type\n", func_name, param->name->str->strval, param->name->str->strval);
        fprintf(output, "JUMP $BODY$%s\n", func_name);
        fprintf(output, "LABEL $WRONG_TYPE$%s\n", func_name);
        if (param->type->str == get_singleton("string"))
            fprintf(output, "PUSHS string@\n");
        else if (param->type->str == get_singleton("int"))
            fprintf(output, "PUSHS int@0\n");
        else if (param->type->str == get_singleton("float"))
            fprintf(output, "PUSHS float@0x0.0p+0\n");
        else if (param->type->str == get_singleton("bool"))
            fprintf(output, "PUSHS bool@false\n");
        fprintf(output, "PUSHS LF@%s\n", param->name->str->strval);
        fprintf(output, "CALL $TO_GOOD_TYPE\n");
        fprintf(output, "POPS LF@%s\n", param->name->str->strval);
        fprintf(output, "POPS LF@%s\n", param->name->str->strval);
    }
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
    data->function = function;
    singleton_t* label = generate_label(function->name);
    fprintf(output, "LABEL $FUNCTION$%s\n", label->strval);
    fprintf(output, "CREATEFRAME\n");
    fprintf(output, "PUSHFRAME\n");

    fprintf(output, "DEFVAR LF@retval\n");
    fprintf(output, "DEFVAR LF@retval$type\n");

    generator_print_variable_declarations(output, function->symtable);

    generator_print_parameter_list(function, __GEN_CREST__);
    fprintf(output, "LABEL $BODY$%s\n", label->strval);
    generator_print_block(function->block, __GEN_CREST__);

    fprintf(output, "POPFRAME\n");
    fprintf(output, "CREATEFRAME\n");
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

void generator_print_mandatory_functions(__GEN_DREST__)
{
    (void)data;
    fprintf(output, GEN_ARIT_CHECK);
    fprintf(output, GEN_ARIT_FUNC);
    fprintf(output, GEN_ASCII_FUN);
    fprintf(output, GEN_BOOLVAL);
    fprintf(output, GEN_FLOATVAL);
    fprintf(output, GEN_INTVAL);
    fprintf(output, GEN_LOGIC_FUN);
    fprintf(output, GEN_STACK_SWITCH);
    fprintf(output, GEN_STRVAL);
    fprintf(output, GEN_TO_GOOD_TYPE);
    fprintf(output, GEN_WRITE);
    fprintf(output, GEN_CONCAT);
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

    fprintf(output, ".IFJcode22\nJUMP $$MAIN\n");
    // generate code for all functions
    if (function_list->next) {
        generator_print_function_list(function_list->next, &data, output);
    }
    generator_print_main(main, &data, output);
    generator_print_mandatory_functions(&data, output);
}

//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace formatter.c: xschie03                //
//    Datum: 28. 11. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./formatter.h"
#include "./error.h"

void formatter_print_space(__FMT_DREST__)
{
    (void)state;
    putc(' ', output);
}

void formatter_print_indent(__FMT_DREST__)
{
    putc('\n', output);

    for (int i = 0; i < state->indentation_level; ++i) {
        fprintf(output, "%s", state->indentation_str);
    }
}

void formatter_print_singleton(singleton_t* singleton, __FMT_DREST__)
{
    (void)state;
    fprintf(output, "%s", singleton->strval);
}

void formatter_print_symbol(symbol_t* symbol, __FMT_DREST__)
{
    (void)state;
    formatter_print_singleton(symbol->str, __FMT_CREST__);
}

void formatter_print_leaf(ast_leaf_t* leaf, __FMT_DREST__)
{
    formatter_print_symbol(leaf->symbol, __FMT_CREST__);

    if (leaf->call_parameters) {
        formatter_print_call_parameter_list(leaf->call_parameters, __FMT_CREST__);
    }
}

void formatter_print_expression(ast_node_t* node, __FMT_DREST__)
{
    if (node->leaf) {
        formatter_print_leaf(node->leaf, __FMT_CREST__);
    } else {
        putc('(', output);
        formatter_print_expression(node->left, __FMT_CREST__);
        formatter_print_space(__FMT_CREST__);
        formatter_print_symbol(node->op, __FMT_CREST__);
        formatter_print_space(__FMT_CREST__);
        formatter_print_expression(node->right, __FMT_CREST__);
        putc(')', output);
    }
}

void formatter_print_statement(ast_node_t* node, __FMT_DREST__)
{
    formatter_print_expression(node, __FMT_CREST__);

    putc(';', output);
}

void formatter_print_conditional(ast_conditional_t* conditional, __FMT_DREST__)
{
    fprintf(output, "if (");
    formatter_print_expression(conditional->condition, __FMT_CREST__);
    fprintf(output, ") ");

    formatter_print_block(conditional->true_branch, __FMT_CREST__);

    if (conditional->false_branch) {
        fprintf(output, " else ");
        formatter_print_block(conditional->false_branch, __FMT_CREST__);
    }
}

void formatter_print_loop(ast_loop_t* loop, __FMT_DREST__)
{
    fprintf(output, "for (");

    if (loop->initializer) {
        formatter_print_expression(loop->initializer, __FMT_CREST__);
    } else {
        putc(' ', output);
    }

    fprintf(output, "; ");

    if (loop->condition) {
        formatter_print_expression(loop->condition, __FMT_CREST__);
    } else {
        putc(' ', output);
    }

    fprintf(output, "; ");

    if (loop->incrementer) {
        formatter_print_expression(loop->incrementer, __FMT_CREST__);
    }

    fprintf(output, ") ");

    formatter_print_block(loop->body, __FMT_CREST__);
}

void formatter_print_block_item(ast_block_item_t* item, __FMT_DREST__)
{
    if (item->item) {
        formatter_print_statement(item->item, __FMT_CREST__);
    } else if (item->conditional) {
        formatter_print_conditional(item->conditional, __FMT_CREST__);
    } else if (item->loop) {
        formatter_print_loop(item->loop, __FMT_CREST__);
    } else {
        throw_warning(99, "formatter: empty block item");
    }
}

void formatter_print_block_contents(ast_block_t* block, __FMT_DREST__)
{
    for (ast_block_item_t* item = block->first; item != NULL; item = item->next) {
        formatter_print_indent(__FMT_CREST__);
        formatter_print_block_item(item, __FMT_CREST__);
    }
}

void formatter_print_block(ast_block_t* block, __FMT_DREST__)
{
    putc('{', output);

    ++state->indentation_level;

    formatter_print_block_contents(block, __FMT_CREST__);

    --state->indentation_level;

    formatter_print_indent(__FMT_CREST__);

    putc('}', output);
}

void formatter_print_call_parameter(ast_call_parameter_t* param, __FMT_DREST__)
{
    formatter_print_expression(param->node, __FMT_CREST__);
}

void formatter_print_call_parameter_list(ast_call_parameter_list_t* list, __FMT_DREST__)
{
    putc('(', output);

    for (size_t i = 0; i < list->size; ++i) {
        if (i) {
            fprintf(output, ", ");
        }

        formatter_print_call_parameter(list->parameters[i], __FMT_CREST__);
    }

    putc(')', output);
}

void formatter_print_parameter(ast_parameter_t* param, __FMT_DREST__)
{
    (void)state;

    if (param->optional) {
        formatter_print_space(__FMT_CREST__);
    }

    formatter_print_symbol(param->type, __FMT_CREST__);
    formatter_print_space(__FMT_CREST__);
    formatter_print_symbol(param->name, __FMT_CREST__);
}

void formatter_print_parameter_list(ast_parameter_list_t* list, __FMT_DREST__)
{
    (void)state;

    putc('(', output);

    for (size_t i = 0; i < list->count; ++i) {
        if (i) {
            fprintf(output, ", ");
        }

        formatter_print_parameter(list->parameters[i], __FMT_CREST__);
    }

    putc(')', output);
}

void formatter_print_function(ast_function_t* fn, __FMT_DREST__)
{
    fprintf(output, "function");
    formatter_print_space(__FMT_CREST__);
    formatter_print_symbol(fn->name, __FMT_CREST__);
    formatter_print_space(__FMT_CREST__);
    formatter_print_parameter_list(fn->parameters, __FMT_CREST__);
    formatter_print_space(__FMT_CREST__);
    formatter_print_block(fn->block, __FMT_CREST__);
}

void formatter_print_function_list(ast_function_list_t* list, __FMT_DREST__)
{
    formatter_print_function(list->item, __FMT_CREST__);

    putc('\n', output);

    if (list->next) {
        putc('\n', output);

        formatter_print_function_list(list->next, __FMT_CREST__);
    }
}

void formatter(ast_function_list_t* list, const char* indentation_str, FILE* output)
{
    formatter_state_t state = {
        .indentation_str = indentation_str,
        .indentation_level = 0,
    };

    fprintf(output, "<?php\n\ndeclare(strict_types=true);\n\n");

    ast_function_t* main = list->item;

    if (list->next) {
        formatter_print_function_list(list->next, &state, output);
    }

    formatter_print_block_contents(main->block, &state, output);

    fprintf(output, "\n?>");
}

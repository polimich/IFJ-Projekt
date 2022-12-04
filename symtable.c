//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace symtable.c: xschie03                 //
//    Datum: 02. 12. 2022 - 03. 12. 2022                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./symtable.h"
#include "./ast.h"
#include "./error.h"

void symtable_insert(symbol_t* symbol, symtable_t* table)
{
    do {
        if (table->str == NULL) {
            table->str = symbol->str;
            table->symbol = symbol;
            return;
        } else if (symbol->str == table->str) {
            throw_warning(0, "Double symtable insert (%s)", symbol->str->strval);
            return;
        } else if (symbol->str < table->str) {
            if (table->lnode == NULL) {
                salloc(symtable_t, lnode);
                table->lnode = lnode;
            }

            table = table->lnode;
        } else if (symbol->str > table->str) {
            if (table->rnode == NULL) {
                salloc(symtable_t, rnode);
                table->rnode = rnode;
            }

            table = table->rnode;
        } else {
            throw_error(99, "symtable_insert impossible condition met");
        }
    } while (table->symbol != symbol);
}

symbol_t* symtable_get(singleton_t* str, symtable_t* table)
{
    while (table != NULL) {
        if (table->str == str) {
            return table->symbol;
        } else if (table->str > str) {
            table = table->lnode;
        } else {
            table = table->rnode;
        }
    }

    return false;
}

STB_PROCESS_DECLARE(call_parameter)
{
    STB_PROCESS_CALL(node, call_parameter->node);
}

STB_PROCESS_DECLARE(call_parameter_list)
{
    for (size_t i = 0; i < call_parameter_list->size; ++i) {
        STB_PROCESS_CALL(call_parameter, call_parameter_list->parameters[i]);
    }
}

STB_PROCESS_DECLARE(leaf)
{
    (void)fn;
    (void)list;
    (void)table;

    symbol_t* symbol = symtable_get(leaf->symbol->str, table);

    if (symbol == NULL) {
        if (leaf->symbol->str->strval[0] == '$') {
            throw_error(5, "Undefined variable %s on line %d", leaf->symbol->str->strval, leaf->symbol->line_number);
        }
    }

    if (leaf->call_parameters) {
        STB_PROCESS_CALL(call_parameter_list, leaf->call_parameters);
    }
}

STB_PROCESS_DECLARE(node)
{
    if (node->leaf) {
        STB_PROCESS_CALL(leaf, node->leaf);
    } else if (node->op->str->strval[0] == '=') {
        // call rvalue first
        STB_PROCESS_CALL(node, node->right);

        // assignment; add lvalue to symtable
        int lvalue_line_number = node->left->leaf->symbol->line_number;
        node->left->leaf->symbol = get_symbol_scoped(symbol_type_local_variable, node->left->leaf->symbol->str, fn->name);
        node->left->leaf->symbol->line_number = lvalue_line_number;

        if (!symtable_get(node->left->leaf->symbol->str, table)) {
            symtable_insert(node->left->leaf->symbol, table);
        }
    } else {
        STB_PROCESS_CALL(node, node->left);
        STB_PROCESS_CALL(node, node->right);
    }
}

STB_PROCESS_DECLARE(conditional)
{
    STB_PROCESS_CALL(node, conditional->condition);

    STB_PROCESS_CALL(block, conditional->true_branch);

    if (conditional->false_branch) {
        STB_PROCESS_CALL(block, conditional->false_branch);
    }
}

STB_PROCESS_DECLARE(loop)
{
    if (loop->initializer) {
        STB_PROCESS_CALL(node, loop->initializer);
    }

    if (loop->condition) {
        STB_PROCESS_CALL(node, loop->condition);
    }

    if (loop->incrementer) {
        STB_PROCESS_CALL(node, loop->incrementer);
    }

    STB_PROCESS_CALL(block, loop->body);
}

STB_PROCESS_DECLARE(block_item)
{
    if (block_item->loop) {
        STB_PROCESS_CALL(loop, block_item->loop);
    } else if (block_item->conditional) {
        STB_PROCESS_CALL(conditional, block_item->conditional);
    } else {
        STB_PROCESS_CALL(node, block_item->item);
    }
}

STB_PROCESS_DECLARE(block)
{
    for (ast_block_item_t* item = block->first; item != NULL; item = item->next) {
        STB_PROCESS_CALL(block_item, item);
    }
}

void symtable_generate(__STB_DREST__)
{
    // add builting functions
    symtable_insert(get_symbol_by_str(symbol_type_function_identifier, "readi"), table);
    symtable_insert(get_symbol_by_str(symbol_type_function_identifier, "readf"), table);
    symtable_insert(get_symbol_by_str(symbol_type_function_identifier, "reads"), table);
    symtable_insert(get_symbol_by_str(symbol_type_function_identifier, "write"), table);

    for (ast_function_list_t* L = list; L != NULL; L = L->next) {
        ast_function_t* F = L->item;

        if (F->name) {
            if (symtable_get(F->name->str, table)) {
                throw_error(3, "Illegal function redefinition on line %d", F->name->line_number);
            }

            symtable_insert(F->name, table);
        }
    }

    if (fn->parameters) {
        for (size_t i = 0; i < fn->parameters->count; ++i) {
            ast_parameter_t* parameter = fn->parameters->parameters[i];

            if (symtable_get(parameter->name->str, table)) {
                throw_error(4, "Duplicite parameter %s in function %s", parameter->name->str->strval, fn->name->str->strval);
            }

            if (parameter->name->str->strval[0] != '$') {
                throw_error(2, "SyntaxError: invalid parameter name %s in function %s", parameter->name->str->strval, fn->name->str->strval);
            }

            parameter->name = get_symbol_scoped(symbol_type_local_variable, parameter->name->str, fn->name);

            symtable_insert(parameter->name, table);
        }
    }

    STB_PROCESS_CALL(block, fn->block);
}

void symtable_init(ast_function_list_t* list)
{

    for (ast_function_list_t* L = list; L != NULL; L = L->next) {
        salloc(symtable_t, table);

        ast_function_t* fn = L->item;
        symtable_generate(__STB_CREST__);

        fn->symtable = table;
    }
}

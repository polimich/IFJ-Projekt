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
            throw_warning(0, "Double symtable insert (%s)", symbol->str);
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

void symtable_generate(ast_function_t* fn, __STB_DREST__)
{
    for (ast_function_list_t* L = list; L != NULL; L = L->next) {
        ast_function_t* F = L->item;

        if (F->name) {
            symtable_insert(F->name, table);
        }
    }

    if (fn->parameters) {
        for (size_t i = 0; i < fn->parameters->count; ++i) {
            ast_parameter_t* parameter = fn->parameters->parameters[i];

            if (symtable_get(parameter->name->str, table)) {
                throw_error(4, "Duplicite parameter %s in function %s", parameter->name->str->strval, fn->name->str->strval);
            }

            parameter->name = get_symbol_scoped(symbol_type_local_variable, parameter->name->str, fn->name);

            symtable_insert(parameter->name, table);
        }
    }
}

void symtable_init(ast_function_list_t* list)
{

    for (ast_function_list_t* L = list; L != NULL; L = L->next) {
        salloc(symtable_t, table);

        ast_function_t* fn = L->item;
        symtable_generate(fn, __STB_CREST__);

        fn->symtable = table;
    }
}

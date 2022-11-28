//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace main.c: xschie03                     //
//    Datum: 24. 10. 2022 - 24. 11. 2022                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./main.h"

void print_ast_node(ast_node_t* node)
{
    if (node->leaf) {
        printf("%s", node->leaf->symbol->str->strval);
        if (node->leaf->call_parameters) {
            printf("(\x1b[B");
            for (size_t i = 0; i < node->leaf->call_parameters->size; ++i) {
                if (i)
                    printf(",");

                print_ast_node(node->leaf->call_parameters->parameters[i]->node);
            }
            printf("\x1b[A)");
        }
    } else {
        printf(" ( \x1b[B");
        print_ast_node(node->left);
        printf(" %s ", node->op->str->strval);
        print_ast_node(node->right);
        printf(" \x1b[A) ");
    }
}

int main(const int argc, const char* argv[])
{
    mem_init();

    utf8_readstream_t* utf8_stdin = utf8_init(stdin);

    if ((argc > 1) && (strcmp(argv[1], "--scanner") == 0)) {
        int line;
        singleton_t* token = NULL;

        singleton_t* end = get_singleton("?>");

        for (int counter = 0; token != end; ++counter) {
            token = lexer_get_token(utf8_stdin, &line);

            fprintf(stdout, "%d\t%d\t%s\n", counter, line, token->strval);
        }
    } else /*if ((argc > 1) && (strcmp(argv[1], "--parser") == 0)) */ {

        formatter(parser(utf8_stdin), "    ", stdout);
    }

    mem_clean();

    return get_current_error_code();
}

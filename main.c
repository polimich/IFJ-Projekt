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

        return 0;
    }

    ast_function_list_t* list = parser(utf8_stdin);

    symtable_init(list);
    semantic_check(list);

    if ((argc > 1) && (strcmp(argv[1], "fmt") == 0)) {
        formatter(list, "    ", stdout);
    } else {
        generator(list, stdout);
    }

    mem_clean();

    return get_current_error_code();
}

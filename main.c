//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace main.c: xschie03                     //
//    Datum: 24. 10. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./main.h"

int main(const int argc, const char* argv[])
{
    mem_init();

    if ((argc > 1) && (strcmp(argv[1], "--scanner") == 0)) {
        int line;
        singleton_t* token = NULL;

        singleton_t* end = get_singleton("?>");

        for (int counter = 0; token != end; ++counter) {
            token = lexer_get_token(stdin, &line);

            fprintf(stdout, "%d\t%d\t%s\n", counter, line, token->strval);
        }
    }

    mem_clean();

    return get_current_error_code();
}

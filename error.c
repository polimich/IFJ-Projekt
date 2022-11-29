//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace error.c: xschie03                    //
//    Datum: 19. 10. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./error.h"
#include "./varstring.h"

#define COLOR_RED "\x1b[31m"
#define COLOR_ORANGE "\x1b[33m"
#define COLOR_YELLOW "\x1b[93m"
#define COLOR_RESET "\x1b[0m"

int CURRENT_RETURN_CODE = 0;

int get_current_error_code()
{
    return CURRENT_RETURN_CODE;
}

void print_error(int error_code, const char* hfmt, const char* fmt, va_list args)
{
    varstring_t* errmsg = varstring_init();

    fprintf(errmsg->stream, hfmt, error_code);

    vfprintf(errmsg->stream, fmt, args);

    fprintf(errmsg->stream, COLOR_RESET);

    fprintf(stderr, "%s\n", varstring_destroy(errmsg)->strval);
}

void throw_warning(int error_code, const char* fmt, ...)
{
    if (CURRENT_RETURN_CODE == 0) {
        CURRENT_RETURN_CODE = error_code;
    }

    va_list args;
    va_start(args, fmt);
    print_error(error_code, (COLOR_ORANGE "Warning(" COLOR_YELLOW "%d" COLOR_ORANGE "): " COLOR_YELLOW), fmt, args);
    va_end(args);
}

void throw_error(int error_code, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    print_error(error_code, (COLOR_RED "  Error(" COLOR_YELLOW "%d" COLOR_RED "): " COLOR_YELLOW), fmt, args);
    va_end(args);

    exit(CURRENT_RETURN_CODE ? CURRENT_RETURN_CODE : error_code);
}

void runtime_assert(int condition, int error_code, const char* fmt, ...)
{
    if (condition == 0) {
        if (CURRENT_RETURN_CODE == 0) {
            CURRENT_RETURN_CODE = error_code;
        }

        va_list args;
        va_start(args, fmt);
        print_error(error_code, (COLOR_ORANGE "Warning(" COLOR_YELLOW "%d" COLOR_ORANGE "): " COLOR_YELLOW "Assertion failed: "), fmt, args);
        va_end(args);
    }
}

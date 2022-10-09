//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace varstring.h: xschie03                //
//    Datum: 9. 10. 2022                                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __VARSTRING_H__
#define __VARSTRING_H__

#include "./singleton.h"

typedef struct varstring_t {
    FILE* stream;
    size_t length;
    char* data;
} varstring_t;

#include <stdarg.h>

inline varstring_t* varstring_init()
{
    varstring_t* str = (varstring_t*)mem_alloc(sizeof(varstring_t));

    str->stream = open_memstream(&str->data, &str->length);

    return str;
}

inline singleton_t* varstring_read(varstring_t* str)
{
    fflush(str->stream);

    return get_singleton(str->data);
}

inline void varstring_write(varstring_t* str, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(str->stream, fmt, args);
    va_end(args);
}

inline singleton_t* varstring_destroy(varstring_t* str)
{
    singleton_t* singleton = varstring_read(str);

    fclose(str->stream);
    free(str->data);
    mem_free((void*)str);

    return singleton;
}

#endif

//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace decoder.c: xschie03                  //
//    Datum: 05. 12. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include <ctype.h>

#include "./encoder.h"
#include "./varstring.h"

void encoder_write_character(int c, varstring_t* s)
{
    if (isalnum(c)) {
        putc(c, s->stream);
    } else if (c >= 100) {
        fprintf(s->stream, "\\%d", c);
    } else if (c >= 10) {
        fprintf(s->stream, "\\0%d", c);
    } else {
        fprintf(s->stream, "\\00%d", c);
    }
}

singleton_t* encoder_encode_string(singleton_t* str)
{
    const char* in = str->strval;
    varstring_t* out = varstring_init();

    varstring_write(out, "string@");

    while (*in) {
        encoder_write_character(((in[1] - '0') * 64 + (in[2] - '0') * 8 + (in[3] - '0') * 1), out);
        in += 4;
    }

    return varstring_destroy(out);
}

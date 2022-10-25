//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace utf8.c: xschie03                     //
//    Datum: 24. 10. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./utf8.h"

extern inline utf8_readstream_t* utf8_init(FILE* stream); // get utf8 stream from FILE*
extern inline void utf8_end(utf8_readstream_t* stream); // free utf8 struct
extern inline void utf8_close(utf8_readstream_t* stream); // free utf8 struct and close file

extern inline int utf8_getc(utf8_readstream_t* stream);
extern inline void utf8_ungetc(int c, utf8_readstream_t* stream);
extern inline void utf8_putc(int c, FILE* stream);

const char* utf8_encode_int(int c)
{
    static char buffer[32] = { 0 };
    static unsigned char offset = 0;

    char* base = &buffer[offset];
    char* wp = base;

    if (c >= 0x10000) {
        *wp++ = (((c >> 18) & 0x07) | 0xF0);
        *wp++ = (((c >> 12) & 0x3F) | 0x80);
        *wp++ = (((c >> 06) & 0x3F) | 0x80);
        *wp++ = (((c >> 00) & 0x3F) | 0x80);
    } else if (c >= 0x800) {
        *wp++ = (((c >> 12) & 0x0F) | 0xE0);
        *wp++ = (((c >> 06) & 0x3F) | 0x80);
        *wp++ = (((c >> 00) & 0x3F) | 0x80);
    } else if (c >= 0x80) {
        *wp++ = (((c >> 06) & 0x1F) | 0xC0);
        *wp++ = (((c >> 00) & 0x3F) | 0x80);
    } else {
        *wp++ = (c);
    }

    *wp++ = '\0';

    offset += (wp - base);
    offset %= 28;

    return base;
}

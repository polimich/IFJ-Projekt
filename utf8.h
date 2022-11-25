//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace utf8.h: xschie03                     //
//    Datum: 24. 10. 2022 - 25. 10. 2022                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __UTF8_H__
#define __UTF8_H__

#include <stdio.h>

#include "./memory.h"

typedef struct utf8_readstream_t {
    FILE* stream;
    int c;
} utf8_readstream_t;

inline utf8_readstream_t* utf8_init(FILE* stream); // get utf8 stream from FILE*
inline void utf8_end(utf8_readstream_t* stream); // free utf8 struct
inline void utf8_close(utf8_readstream_t* stream); // free utf8 struct and close file

inline int utf8_getc(utf8_readstream_t* stream);
inline void utf8_ungetc(int c, utf8_readstream_t* stream);
inline void utf8_putc(int c, FILE* stream);

inline utf8_readstream_t* utf8_init(FILE* stream)
{
    salloc(utf8_readstream_t, descriptor);

    descriptor->stream = stream;

    return descriptor;
}

inline void utf8_end(utf8_readstream_t* stream)
{
    if (stream->c > 0) {
        ungetc(stream->c, stream->stream);
    }

    mem_free((void*)stream);
}

inline void utf8_close(utf8_readstream_t* stream)
{
    FILE* file = stream->stream;

    utf8_end(stream);

    fclose(file);
}

inline int utf8_getc(utf8_readstream_t* stream)
{
    int c = 0;

    if (stream->c > 0) {
        c = stream->c;
        stream->c = 0;
    } else {
        c = getc(stream->stream);

        if (c >= 0xF0) {
            c &= 0xF;
            c <<= 18;
            c |= (getc(stream->stream) & 0x3F) << 12;
            c |= (getc(stream->stream) & 0x3F) << 06;
            c |= (getc(stream->stream) & 0x3F) << 00;
        } else if (c >= 0xE0) {
            c &= 0x1F;
            c <<= 12;
            c |= (getc(stream->stream) & 0x3F) << 06;
            c |= (getc(stream->stream) & 0x3F) << 00;
        } else if (c >= 0xC0) {
            c &= 0x1F;
            c <<= 6;
            c |= (getc(stream->stream) & 0x3F) << 00;
        }
    }

    return c;
}

inline void utf8_ungetc(int c, utf8_readstream_t* stream)
{
    stream->c = c;
}

inline void utf8_putc(int c, FILE* stream)
{
    if (c >= 0x10000) {
        putc(((c >> 18) & 0x07) | 0xF0, stream);
        putc(((c >> 12) & 0x3F) | 0x80, stream);
        putc(((c >> 06) & 0x3F) | 0x80, stream);
        putc(((c >> 00) & 0x3F) | 0x80, stream);
    } else if (c >= 0x800) {
        putc(((c >> 12) & 0x0F) | 0xE0, stream);
        putc(((c >> 06) & 0x3F) | 0x80, stream);
        putc(((c >> 00) & 0x3F) | 0x80, stream);
    } else if (c >= 0x80) {
        putc(((c >> 06) & 0x1F) | 0xC0, stream);
        putc(((c >> 00) & 0x3F) | 0x80, stream);
    } else {
        putc(c, stream);
    }
}

const char* utf8_encode_int(int c);

#define utf8_isdigit(c) (c >= '0' && c <= '9')
#define utf8_isalpha(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define utf8_isalnum(c) (utf8_isdigit(c) || utf8_isalpha(c))

#endif

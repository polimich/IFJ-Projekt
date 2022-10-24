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

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

#include "./decoder.h"
#include "./error.h"
#include "./varstring.h"

void decoder_write_character(int c, varstring_t* s)
{
    if (c >= 0x200 || c < 0) {
        throw_warning(99, "Invalid character: %x", c);
        c &= 0x1ff;
    }

    if (c >= 64) {
        fprintf(s->stream, "\\%o", c);
    } else if (c >= 8) {
        fprintf(s->stream, "\\0%o", c);
    } else {
        fprintf(s->stream, "\\00%o", c);
    }
}

singleton_t* decoder_decode_string(singleton_t* str)
{
    const char* in = str->strval;
    varstring_t* out = varstring_init();

    if (*in != '"') {
        throw_warning(99, "string without quotation marks passed into decoder_decode_string(%s)", str->strval);
    } else {
        in++; // discard '"'
    }

    while (*in && *in != '"') {
        if (*in == '$') {
            throw_warning(1, "Cannot use unescaped '$' in strings.");
            decoder_write_character('$', out);
            in++;
        } else if (*in == '\\') {
            if (in[1] == '\\') {
                decoder_write_character('\\', out);
                in += 2;
            } else if (in[1] == 't') {
                decoder_write_character('\t', out);
                in += 2;
            } else if (in[1] == 'n') {
                decoder_write_character('\n', out);
                in += 2;
            } else if (in[1] == '"') {
                decoder_write_character('"', out);
                in += 2;
            } else if (in[1] == '$') {
                decoder_write_character('$', out);
                in += 2;
            } else if (in[1] == 'x') {
                if (is_hex_digit(in[2])) {
                    if (is_hex_digit(in[3])) {
                        int c = read_hex_digit(in[2]) * 16 + read_hex_digit(in[3]);
                        decoder_write_character(c, out);
                        in += 4;
                    } else {
                        int c = read_hex_digit(in[2]);
                        decoder_write_character(c, out);
                        in += 3;
                    }
                } else {
                    decoder_write_character(*(in++), out);
                    decoder_write_character(*(in++), out);
                }
            } else if (is_octal_digit(in[1]) && is_octal_digit(in[2]) && is_octal_digit(in[3])) {
                decoder_write_character(((in[1] - '0') * 64 + (in[2] - '0') * 8 + (in[3] - '0') * 1), out);
                in += 4;
            } else {
                decoder_write_character(*(in++), out);
            }
        } else {
            decoder_write_character(*(in++), out);
        }
    }

    return varstring_destroy(out);
}

//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace error.h: xschie03                    //
//    Datum: 19. 10. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __ERROR_H__
#define __ERROR_H__

#define ERROR_CODE_LEXICAL (1)
#define ERROR_CODE_SYNTAX (2)
#define ERROR_CODE_UNDEFINED_OR_REDEFINED_FUNCTION (3)
#define ERROR_CODE_RUNTIME_TYPE_ERROR (4)
#define ERROR_CODE_UNDEFINED_VARIABLE (5)
#define ERROR_CODE_BAD_RETURN_VALUE (6)
#define ERROR_CODE_ARITHMETIC_STRING_RELATION_TYPE_ERROR (7)
#define ERROR_CODE_SEMANTIC_OTHER (8)
#define ERROR_CODE_INTERNAL (99)

int get_current_error_code();

void throw_warning(int error_code, const char* fmt, ...);
void throw_error(int error_code, const char* fmt, ...);

#endif

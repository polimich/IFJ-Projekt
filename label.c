//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace label.c: xschie03                    //
//    Datum: 25. 11. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./label.h"
#include "./varstring.h"

singleton_t* generate_label(void* ptr)
{
    varstring_t* str = varstring_init();

    varstring_write(str, "label_%llx", (long long unsigned int)ptr);

    return varstring_destroy(str);
}

//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace singleton.c: xschie03                //
//    Datum: 4. 10. 2022                                //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#include "./singleton.h"

#define SINGLETON_HASHMAP_SIZE (4096)

singleton_t* singleton_hashmap[SINGLETON_HASHMAP_SIZE] = { NULL };

singleton_t* find_singleton(singleton_t* singleton, const char* restrict strval)
{
    while (singleton != NULL) {
        if (!strcmp(singleton->strval, strval)) {
            return singleton;
        }

        singleton = singleton->next;
    }

    return singleton;
}

singleton_t* get_singleton(const char* restrict strval)
{
    size_t strhash = hash_f(strval);
    size_t strhash_mod = strhash % SINGLETON_HASHMAP_SIZE;

    singleton_t* existing = find_singleton(singleton_hashmap[strhash_mod], strval);

    if (existing) {
        return existing;
    } else {
        char* strval_cpy = mem_alloc_safe(strlen(strval) + 1);

        strcpy(strval_cpy, strval);

        singleton_t* singleton = (singleton_t*)mem_alloc_safe(sizeof(singleton_t));

        singleton->hash = strhash;
        singleton->strval = strval_cpy;

        singleton->next = singleton_hashmap[strhash_mod];
        singleton_hashmap[strhash_mod] = singleton;

        return singleton;
    }
}
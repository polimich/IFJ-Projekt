#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef MEMDEBUG

void* mem_alloc(size_t size);
void mem_free(void* pointer);

void mem_init();
void mem_clean();

#else

#define mem_alloc(size) (calloc((size) + 1, 1))
#define mem_free(pointer) (free((pointer)))

#define mem_init() (0)
#define mem_clean() (0)

#endif

inline void* mem_alloc_safe(size_t size)
{
    void* ptr = mem_alloc(size);

    if (ptr == NULL) {
        perror("Memory allocation error");
        exit(99);
    }

    return ptr;
}

#endif

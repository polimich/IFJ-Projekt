#ifndef MEMDEBUG
#define MEMNDEBUG
#define MEMDEBUG
#endif

#include "./memory.h"

#ifdef MEMNDEBUG
#undef MEMNDEBUG
#undef MEMDEBUG
#endif

#ifndef MEM_FILE
#define MEM_FILE "memory.dbg"
#endif

#ifndef MEM_SIZE
#define MEM_SIZE 0x100000000L
#endif

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

void* field = NULL;
ptrdiff_t offset = 0;

typedef struct mem_t {
    size_t state;
    size_t size;
    char data[];
} mem_t;

void mem_init()
{
    int fd = open((MEM_FILE), O_RDWR | O_CREAT, 0600);

    if (errno) {
        perror("Memory allocation error");
        exit(99);
    }

    truncate((MEM_FILE), (MEM_SIZE));

    if (errno) {
        perror("Memory allocation error");
        exit(99);
    }

    field = mmap(NULL, (MEM_SIZE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (errno || (field == MAP_FAILED)) {
        perror("Memory allocation error");
        exit(99);
    }

    close(fd);
}

void* mem_alloc(size_t size)
{
    mem_t* mem = (mem_t*)(((char*)field) + (offset << 6));

    mem->state = 'a';
    mem->size = size;

    offset += ((size + sizeof(mem_t)) >> 6) + 1;

    return mem->data;
}

void mem_free(void* pointer)
{
    mem_t* mem = ((mem_t*)(pointer)) - 1;

    mem->state = 'f';
}

void mem_clean() { }

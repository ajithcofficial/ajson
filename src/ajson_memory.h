#ifndef PRO_MEMORY_H
#define PRO_MEMORY_H

#include "ajson_linux.h"

typedef struct memory_s memory_t;
struct memory_s
{
    void *context;
    void *(*alloc)(memory_t* mem, size_t size);
    void *(*realloc)(memory_t* mem, void *old, size_t size);
    int (*free)(memory_t* mem, void *addr);
    int (*destroy)(memory_t* mem);
};


memory_t *create_memory(size_t size);
#endif

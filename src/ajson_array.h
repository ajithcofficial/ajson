#ifndef PRO_ARRAY_H
#define PRO_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ajson_memory.h"

typedef struct array_s array_t;
struct array_s 
{
    void *array_context;
    void *(*push)(array_t *array);
    void *(*get_array)(array_t *array);
    size_t (*get_count)(array_t *array);
    int (*destroy)(array_t *array);
    void (*flush)(array_t *array);
};


array_t * create_array(memory_t *mem, size_t size);

#endif


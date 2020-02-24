
#include "ajson_array.h"

#define DEFAULT_ELEMENT_COUNT 10
    
typedef struct array_context_s
{
    size_t count_init;
    size_t count_max;
    size_t count_used;
    size_t size;
    memory_t *memory;
    void *elements;
}array_context_t;

static int ajson_array_destroy(array_t *array)
{
    array_context_t *array_context;

    array_context = array->array_context;
    free(array_context->elements);
    free(array_context);
    free(array);
    return 0;
}
    
static void ajson_array_flush(array_t *array)
{
    array_context_t *array_context;

    array_context = array->array_context;
    array_context->count_used = 0;
}


static size_t ajson_array_count(array_t *array)
{
    array_context_t *array_context;

    array_context = array->array_context;
    return array_context->count_used;
}


static void *ajson_array_get(array_t *array)
{
    array_context_t *array_context;

    array_context = array->array_context;
    return array_context->elements;
}

static void *ajson_array_push(array_t *array)
{
    array_context_t *array_context;
    void *pos;

    array_context = array->array_context;
    if(array_context->count_max <= array_context->count_used) {
	array_context->count_max += array_context->count_init;
	if((array_context->elements = array_context->memory->realloc(array_context->memory, array_context->elements, array_context->count_max * array_context->size)) == NULL) {
	    return NULL;
	}
    }
    pos = array_context->elements + (array_context->count_used * array_context->size);
    array_context->count_used++;
    return pos;
}


array_t * create_array(memory_t *mem, size_t size)
{
    array_t *array;
    array_context_t *array_context;

    if((array = mem->alloc(mem,sizeof(array_t))) == NULL) {
	return NULL;
    }

    if((array_context = mem->alloc(mem,sizeof(array_context_t))) == NULL) {
	free(array);
	return NULL;
    }
    array->array_context = array_context;

    array_context->memory = mem;
    array_context->count_init = DEFAULT_ELEMENT_COUNT;
    array_context->count_max = DEFAULT_ELEMENT_COUNT;
    array_context->count_used = 0;
    array_context->size = size;
    if((array_context->elements = mem->alloc(mem, array_context->count_max * array_context->size)) == NULL) {
	free(array);
	return NULL;
    }

    array->push = ajson_array_push;
    array->get_array = ajson_array_get;
    array->get_count = ajson_array_count;
    array->destroy = ajson_array_destroy;
    array->flush = ajson_array_flush;

    return array;
}

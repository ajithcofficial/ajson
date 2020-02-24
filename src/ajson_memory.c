#include "ajson_memory.h"

typedef struct memory_context_s {
    char *start;
    char *end;
    size_t size;
    int allocations;
    struct memory_context_s *next;
} memory_context_t;

typedef struct memmeta_s {
    void *context;
    short magic;
    short free:1;
    short size:15;
} memmeta_t;


static int ajson_memory_memdestroy(memory_t * memory)
{
    memory_context_t *ctx;

    ctx = memory->context;

    while(ctx) {
        free(ctx);
	ctx = ctx->next;
    }
    return 0;
}

static void* ajson_memory_alloc_context(memory_context_t *ctx, size_t size)
{
    int i;
    memmeta_t *meta;
    char *ret;

    if (size > 32768) {
	return NULL;
    }

    size &= (~0xf);
    for (i = 16; i < 65536; i *= 2) {
	if (!(i & size)) {
	    size |= i;
	    break;
	}
    }
    if ((size + sizeof(memmeta_t)) > ctx->size) {
	return NULL;
    }

    meta = (memmeta_t *) ctx->start;
    for (i = 0; i < ctx->allocations; i++) {
	if (meta->magic != (short) 0xABCD) {
	    raise(SIGSEGV);
	    return NULL;
	}
	if ((meta->free) && ((size_t) meta->size == size)) {
	    ctx->size -= (size + sizeof(memmeta_t));
	    meta->free = 0;
	    ret = ((char *) meta) + sizeof(memmeta_t);
	    return ret;
	}
	meta =
	    (memmeta_t *) ((char *) meta) + meta->size + sizeof(memmeta_t);
    }

    if ((((char *) meta) + sizeof(memmeta_t) + size) > ctx->end) {
	return NULL;
    }
    meta->free = 0;
    meta->size = size;
    meta->magic = (short) 0xABCD;
    meta->context = ctx;
    ctx->size -= (size + sizeof(memmeta_t));
    ctx->allocations++;
    ret = ((char *) meta) + sizeof(memmeta_t);
    return ret;
}

static memory_context_t *create_memory_context(size_t size)
{
    memory_context_t *ctx;

    if((ctx = calloc(1,size)) == NULL) {
	return NULL;
    }
    ctx->start = ((char *) ctx) + sizeof(memory_context_t);
    ctx->end = ((char *) ctx) + size;
    ctx->size = ctx->end - ctx->start;
    ctx->allocations = 0;
    ctx->next = NULL;
    return ctx;
}

void *ajson_memory_alloc(memory_t * memory, size_t size)
{
    char *ret;
    memory_context_t **ctx, *last;

    ctx = (memory_context_t **) &memory->context;

    while(*ctx) {
	last = *ctx;
	if((ret = ajson_memory_alloc_context(*ctx,size)) != NULL) {
	    return ret;
	}
	ctx = &(*ctx)->next;
    }

    if((*ctx = create_memory_context((char*)last->end - (char*)last)) == NULL) {
	return NULL;
    }
	if((ret = ajson_memory_alloc_context(*ctx,size)) != NULL) {
	    return ret;
	}
    return NULL;

}

static int ajson_memory_free(memory_t * memory, void *mem)
{
    memmeta_t *meta;
    memory_context_t *ctx;

    meta = (memmeta_t *) (mem - sizeof(memmeta_t));
    ctx = meta->context;

    if (meta->magic != (short) 0xABCD) {
	return -1;
    }

    meta->free = 1;
    ctx->size += (meta->size + sizeof(memmeta_t));
    return 0;
}


static void *ajson_memory_realloc(memory_t * memory, void *old, size_t size)
{
    printf("not implemented realloc\n");
    return NULL;
}

memory_t *create_memory(size_t size)
{
    memory_t *memory;
    memory_context_t *ctx;

    if((ctx = create_memory_context(size)) == NULL) {
	return NULL;
    }

    if ((memory = ajson_memory_alloc_context(ctx, sizeof(memory_t))) == NULL) {
	free(ctx);
	return NULL;
    }

    memory->context = ctx;
    memory->destroy = ajson_memory_memdestroy;
    memory->alloc = ajson_memory_alloc;
    memory->realloc = ajson_memory_realloc;
    memory->free = ajson_memory_free;
    return memory;
}

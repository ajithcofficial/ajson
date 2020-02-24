#ifndef AJSON_H
#define AJSON_H

#include "ajson_string.h"
#include "ajson_memory.h"


typedef string_t ajson_string_t;

typedef enum {AJSON_NULL = 0, AJSON_OBJECT, AJSON_ARRAY, AJSON_STRING, AJSON_INT, AJSON_BOOL} ajson_type_t;

typedef struct ajson_object_s ajson_object_t;

struct ajson_object_s {
    void *context;

    ajson_object_t *	(*get_child) (ajson_object_t *ajson, const char*name); 
    ajson_string_t *	(*get_value) (ajson_object_t *ajson);
    ajson_string_t *	(*get_name) (ajson_object_t *ajson);
    ajson_type_t *	(*get_type) (ajson_object_t *ajson);
    ajson_string_t *	(*stringify_value) (ajson_object_t *ajson);
    int			(*decode_value) (ajson_object_t *ajson, void *value);
};

ajson_object_t * ajson_parse(const char *json_string);
ajson_object_t * ajson_parse_string(ajson_string_t *json_string);
ajson_object_t * ajson_parse_using_preallocation(const char *json_string, void *ptr, size_t size);
ajson_object_t * ajson_parse_string_using_preallocation(ajson_string_t *json_string, void *ptr, size_t size);

#endif

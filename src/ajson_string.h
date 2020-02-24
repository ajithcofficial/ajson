#ifndef PRO_STRING_H
#define PRO_STRING_H

#include <string.h>
#include <stdlib.h>

//efine ajson_string(string) {string , sizeof(string)-1}

typedef enum {STR_VAR = 0, STR_CONST } strtype_t;

typedef struct string_s string_t;
typedef struct keyvalue_s
{
    string_t *key;
    string_t *value;
}keyvalue_t;

struct string_s
{
    char con;
    char *start;
    int length;
    int (*set)(string_t *str,char *value, size_t len, strtype_t con);
    void (*reset)(string_t *str);
};

string_t *create_string(char *value, size_t len, strtype_t con);

#endif

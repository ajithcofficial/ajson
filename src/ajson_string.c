#include "ajson_string.h"

static void ajson_string_reset(string_t *str)
{
    if(str->con) {
        str->start = NULL;
	str->length = 0;
	return;
    }
    free(str->start);
    str->start = NULL;
    str->length = 0;
    return;
}


static int ajson_string_set(string_t *str, char *value, size_t len, strtype_t con)
{
    ajson_string_reset(str);

    str->con = con;
    if(con) {
        str->start = value;
	str->length = len;
	return 0;
    }

    if((str->start = calloc(1,len+1)) == NULL) {
	return -1;
    }
    strncpy(str->start,value,len);
    str->length = len;
    return 0;
}

string_t *create_string(char *value, size_t len, strtype_t con)
{
    string_t *str;

    if((str = calloc(1,sizeof(string_t))) == NULL) {
	return NULL;
    }
    str->con = con;
    if(con) {
        str->start = value;
	str->length = len;
	str->reset = ajson_string_reset;
	str->set = ajson_string_set;
	return str;
    }

    if((str->start = calloc(1,len+1)) == NULL) {
	free(str);
	return NULL;
    }

    strncpy(str->start,value,len);
    str->length = len;
    str->reset = ajson_string_reset;
    str->set = ajson_string_set;
    return str;
}

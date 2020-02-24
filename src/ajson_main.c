#include <stdio.h>

#include "ajson.h"

int main()
{

//    printf("%.*s\n",4,"asdfghjkl");

    char *str = "{\"student\" : null, \"college\" : \"ssmptc\", \"test\":\"value\", \"body\" :{ \"content\" : \"hai\", \"status\" : \"ok\"}, \"header\" : {\"title\" : \"json\" } }";

    ajson_object_t *ajson = ajson_parse(str);

    ajson_object_t *body = ajson->get_child(ajson,"body");

    ajson_object_t *content = body->get_child(body,"content");

    string_t *name = content->get_name(content);

    printf("name %.*s\n",name->length, name->start);

    string_t *value = ajson->get_value(ajson);

    printf("value %.*s\n",value->length, value->start);

    return 0;
}

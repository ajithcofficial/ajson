#include "ajson.h"
#include "ajson_array.h"


typedef struct ajson_context_s {
    ajson_type_t type; // string, array, object
    string_t *name;
    string_t *value;
    array_t *children;
    memory_t *memory;
} ajson_context_t;



typedef enum { opener, nmstart, name, seperator, vlstart, value,
    closed
} ajson_parse_t;

typedef enum { other =
	0, alpha, number, space, opbraze, clbraze, opbrack, clbrack,
    escape, comma, quote, colon
} chartype_t;


static chartype_t get_chartype(char c)
{
    return (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) * alpha) +
	((c >= '0' && c <= '9') * number) +
	((c == ' ') * space) +
	((c == '{') * opbraze) +
	((c == '}') * clbraze) +
	((c == '[') * opbrack) +
	((c == ']') * clbrack) +
	((c == ',') * comma) +
	((c == '\\') * escape) +
	((c == '\"') * quote) + ((c == ':') * colon);
}

char *ajson_parse_object(char *start, char *end,
		       ajson_context_t * parent)
{
    char *pos;
    ajson_parse_t parse;
    ajson_context_t *ctx;

    pos = start;

    parse = opener;
    start = NULL;
    while (pos < end) {
	switch (parse) {
	case opener:
	    switch (get_chartype(*pos)) {
	    case space:
		break;
	    case opbraze:
		parse = nmstart;
		break;
	    default:
		return NULL;
	    };
	    break;

	case nmstart:
	    switch (get_chartype(*pos)) {
	    case space:
		break;
	    case quote:
		start = pos + 1;
		parse = name;
		break;
	    default:
		return NULL;
	    };
	    break;
	case name:
	    switch (get_chartype(*pos)) {
	    case quote:
		if (get_chartype(*(pos - 1)) == escape) {
		    break;
		}
		if((ctx = (ajson_context_t *) parent->children->push(parent->children)) == NULL) {
		    printf("array push failed\n");
		    return NULL;
		}
		ctx->memory = parent->memory;

		if((ctx->children = create_array(ctx->memory, sizeof(ajson_context_t))) == NULL) {
		    return NULL;
		}
		ctx->name = create_string(start, pos - start, 1);
		
		printf("got name %.*s\n", ctx->name->length,
		       ctx->name->start);
		start = NULL;
		parse = seperator;
	    default:
		break;
	    };
	    break;
	case seperator:
	    switch (get_chartype(*pos)) {
	    case space:
		break;
	    case colon:
		parse = vlstart;
		break;
	    default:
		return NULL;
	    };
	    break;

	case vlstart:
	    switch (get_chartype(*pos)) {
	    case space:
		break;
	    case quote:
		ctx->type = AJSON_STRING;
		printf("type identified string\n");
		parse = value;
		start = pos;
		break;
	    case opbraze:
		printf("type identified object\n");
		ctx->type = AJSON_OBJECT;
		parse = value;
		start = pos;
		break;
	    case opbrack:
		ctx->type = AJSON_ARRAY;
		parse = value;
		start = pos + 1;
		break;
	    case alpha:
		if (*pos == 'n') {
		    ctx->type = AJSON_NULL;
		    printf("type identified null\n");
		    parse = value;
		    start = pos;
		    break;
		}
		if ((*pos == 't') || (*pos == 'f')) {
		    ctx->type = AJSON_BOOL;
		    parse = value;
		    start = pos;
		    break;
		}
		return NULL;
	    case number:
		ctx->type = AJSON_INT;
		parse = value;
		start = pos;
		break;
	    default:
		return NULL;
	    };
	    break;
	case value:
	    switch (ctx->type) {
	    case AJSON_NULL:
		if (pos - start == 1) {
		    if (*pos != 'u') {
			return NULL;
		    }
		    break;
		}
		if ((pos - start == 2) || (pos - start == 3)) {
		    if (*pos != 'l') {
			return NULL;
		    }
		    break;
		}
		if (pos - start > 3) {
		    switch (get_chartype(*pos)) {
		    case space:
			break;
		    case comma:
			ctx->value = create_string(start, pos - start, 1);
			printf("got value %.*s\n", ctx->value->length,
			       ctx->value->start);
			start = NULL;
			parse = nmstart;
			break;
		    case clbraze:
			ctx->value = create_string(start, pos - start, 1);
			printf("got value %.*s\n", ctx->value->length,
			       ctx->value->start);
			start = NULL;
			parse = closed;
			break;
		    default:
			return NULL;
		    };
		}
		break;

	    case AJSON_OBJECT:
		printf("identified object\n");

		if(start != NULL){
		    if((pos = ajson_parse_object(start, end, ctx)) == NULL) {
		        printf("parsing failed %s\n", start);
		        return NULL;
		    }
			ctx->value = create_string(start, pos - start, 1);
			printf("got value %.*s\n", ctx->value->length,
			       ctx->value->start);
			start = NULL;
		}


		switch (get_chartype(*pos)) {
		case comma:
		    parse = nmstart;
		    break;
		case space:
		    break;
		case clbraze:
		    parse = closed;
		    break;
		default:
		    return NULL;
		};
		break;
	    case AJSON_ARRAY:
	    case AJSON_STRING:

		switch (get_chartype(*pos)) {
		case quote:

		    if (!start) {
			return NULL;
		    }

		    if (get_chartype(*(pos - 1)) == escape) {
			break;
		    }
		    ctx->value = create_string(start, pos - start + 1, 1);
		    printf("got value %.*s\n", ctx->value->length,
			   ctx->value->start);
		    start = NULL;
		    break;
		case comma:
		    if (!start) {
			parse = nmstart;
		    }
		    break;
		case clbraze:
		    if (!start) {
			parse = closed;
			break;
		    }
		    break;
		case space:
		    break;
		default:
		    if (!start) {
			return NULL;
		    }
		    break;

		};
		break;

	    case AJSON_INT:
	    case AJSON_BOOL:
	    default:
		return NULL;
	    };
	    break;
	case closed:
	    printf("reach closed\n");
	    return pos;
	default:
	    return NULL;
	};
	pos++;
    }
    return (parse == closed) ? pos : NULL;
}


static ajson_string_t *ajson_get_name (ajson_object_t *ajson)
{
    ajson_context_t *ctx;

    ctx = ajson->context;
    return ctx->name;
}


static ajson_string_t *ajson_get_value (ajson_object_t *ajson)
{
    ajson_context_t *ctx;

    ctx = ajson->context;
    return ctx->value;
}

static ajson_object_t *ajson_get_child (ajson_object_t *ajson, const char*name)
{

    ajson_context_t *ctx, *child;
    ajson_object_t *ret;
    int i,n, len;

    ctx = ajson->context;

    if((child = ctx->children->get_array(ctx->children)) == NULL) {
	return NULL;
    }
    len = strlen(name);
    n = ctx->children->get_count(ctx->children);
    for(i=0; i<n; i++) {
	if((child[i].name->length == len) &&(!strncmp(name, child[i].name->start, len))) {
	    ret = calloc(1,sizeof(ajson_object_t));
	    ret->context = &child[i];
	    ret->get_child = ajson_get_child;
	    ret->get_name = ajson_get_name;
	    ret->get_value = ajson_get_value;
	    return ret;
	}
    }
    return NULL;
}



ajson_object_t *ajson_parse(const char *json_string)
{
    ajson_object_t *ajson;
    ajson_context_t *ctx;
    char *end;
    memory_t *memory;


    if((memory = create_memory(10240)) == NULL) {
	return NULL;
    }


    if ((ajson = memory->alloc(memory, sizeof(ajson_object_t))) == NULL) {
	return NULL;
    }

    if ((ctx = memory->alloc(memory, sizeof(ajson_context_t))) == NULL) {
	free(ajson);
	return NULL;
    }

    ctx->memory = memory;
    if ((ctx->children = create_array(ctx->memory, sizeof(ajson_context_t))) == NULL) {
	free(ajson);
	free(ctx);
	return NULL;
    }

    ajson->context = ctx;
    ajson->get_child = ajson_get_child;
    ajson->get_name = ajson_get_name;
    ajson->get_value = ajson_get_value;

    ctx->type = AJSON_OBJECT;
    ctx->name = create_string("root", sizeof("root")-1, 1);

    if((end = ajson_parse_object((char*) json_string, (char*)json_string + strlen(json_string),
		       ctx)) == NULL) {
	printf("parsing failed\n");
	return NULL;
    }
			ctx->value = create_string((char *)json_string, end - json_string, 1);
			printf("got value %.*s\n", ctx->value->length,
			       ctx->value->start);

    return ajson;
}

ajson_object_t *ajson_parse_string(ajson_string_t * json_string)
{
    return NULL;
}

ajson_object_t *ajson_parse_using_preallocation(const char *json_string,
						void *ptr, size_t size)
{
    return NULL;
}

ajson_object_t *ajson_parse_string_using_preallocation(ajson_string_t *
						       json_string,
						       void *ptr,
						       size_t size)
{
    return NULL;
}

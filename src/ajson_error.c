#include "ajson_error.h"


#define ERR_MAX 255

typedef struct error_context_s {
    char *e_msg;
    size_t e_len;
    time_t e_time;
    int e_sys;
    size_t e_max;
} error_context_t;

#define ERR_TOO_LARGE "error message too large"
#define ERR_MSG_UNAVAIL "error message unavailable"


static int ajson_set_error(error_t * err, char *message)
{
    error_context_t *error_context = err->error_context;
    size_t n;

    if ((n = strlen(message)) >= error_context->e_max) {
	strcpy(error_context->e_msg, ERR_TOO_LARGE);
	error_context->e_len = sizeof(ERR_TOO_LARGE) - 1;
	error_context->e_time = time(NULL);
	error_context->e_sys = errno;
	return -1;
    }
    strcpy(error_context->e_msg, message);
    error_context->e_len = n;
    error_context->e_time = time(NULL);
    error_context->e_sys = errno;
    return 0;
}

static size_t ajson_get_error_len(error_t * error)
{
    error_context_t *error_context = error->error_context;
    return error_context->e_len;
}

static time_t ajson_get_error_time(error_t * error)
{
    error_context_t *error_context = error->error_context;
    return error_context->e_time;
}

static int ajson_get_error_message(error_t * error, char *message,
				 size_t len)
{
    error_context_t *error_context = error->error_context;
    strncpy(message, error_context->e_msg, len - 1);
    return 0;
}

static int ajson_print_error(error_t * error)
{
    error_context_t *error_context = error->error_context;
    printf("Server message: %s, System message: %s, Time : %s",
	   error_context->e_msg, strerror(error_context->e_sys),
	   ctime(&error_context->e_time));
    return 0;
}

static int ajson_get_system_code(error_t * error)
{
    error_context_t *error_context = error->error_context;
    return error_context->e_sys;
}

error_t *create_error(size_t error_max)
{
    error_t *error;
    error_context_t *error_context;

    if ((error = calloc(1, sizeof(error_t))) == NULL) {
	return NULL;
    }
    if ((error_context = calloc(1, sizeof(error_context_t))) == NULL) {
	return NULL;
    }

    error->error_context = error_context;
    error_context->e_max = error_max;

    if ((error_context->e_msg = calloc(1, error_context->e_max)) == NULL) {
	return NULL;
    }
    strcpy(error_context->e_msg,ERR_MSG_UNAVAIL);

    error->set_error = ajson_set_error;
    error->get_error_len = ajson_get_error_len;
    error->get_error_time = ajson_get_error_time;
    error->get_error_message = ajson_get_error_message;
    error->print_error = ajson_print_error;
    error->get_system_code = ajson_get_system_code;

    return error;
}


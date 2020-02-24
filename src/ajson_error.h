#ifndef PRO_ERROR_H
#define PRO_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

typedef struct error_s error_t;

struct error_s
{
    void *error_context;

    int (*set_error)(error_t *err,char *message);
    size_t (*get_error_len)(error_t *err);
    time_t (*get_error_time)(error_t *err);
    int (*get_error_message)(error_t *err,char *message,size_t len);
    int (*print_error)(error_t *err);
    int (*get_system_code)(error_t *err);
};

extern error_t *create_error();

#endif

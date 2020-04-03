#ifndef PARSER_H
#define PARSER_H

#include "commands.h"

#define DELIM " \t\r\n"
#define DELIM_WITH_NULL " \t\r\n\x00"

enum parsing_error_type {
    NO_COMMAND = 0,
    UNAVAILABLE_COMMAND = 1,
    INCORRECT_NUM_PARAMS = 2,
    INCORRECT_TYPE = 3,
    INCORRECT_RANGE = 4
};

typedef struct _parsing_errors {
    enum parsing_error_type error_type;
    int param_index;
} parsing_errors_t;

int parse_command(const char *text, enum sudoku_mode mode, const command_list_t *commands, command_t **command_ptr,
        command_arguments_t *args, parsing_errors_t *errors);

#endif

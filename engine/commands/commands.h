#ifndef SUDOKU_COMMANDS_H
#define SUDOKU_COMMANDS_H

#include <stdio.h>

#include "../sudoku_game.h"

#define MAX_ARGS (3)

/**
 * Argument type, integer or string
 */
enum argument_type {INTEGER = 0, STRING = 1, FLOAT = 2};

/**
 * A command argument description, includes type and range (for integers).
 * Used for validating validity of input and casting to the appropriate type.
 */
typedef struct _command_argument_description {
    enum argument_type argument_type;
    int optional;

    /* Only for integers */
    int enforce_range;
    union _lower_bound {
        int int_value;
        float float_value;
    } lower_bound;
    union _upper_bound {
        int int_value;
        float float_value;
    } upper_bound;
} command_argument_description_t;

/**
 * A command argument list description.
 */
typedef struct _command_arguments_description {
    int num_arguments;
    command_argument_description_t* arguments[MAX_ARGS];
} command_arguments_description_t;

/**
 * A command argument, includes type and value (as a pointer)
 */
typedef struct _command_argument {
    enum argument_type type;
    int supplied;
    union value_t {
        char *str_value;
        int int_value;
        float float_value;
    } value;
} command_argument_t;

/**
 * List of arguments passed to a command.
 */
typedef struct _command_arguments {
    int num_arguments;
    command_argument_t arguments[MAX_ARGS];
} command_arguments_t;

/**
 * Modes a command is available in
 */
typedef struct _modes_availability {
    int init;
    int edit;
    int solve;
} modes_availability_t;

enum command_status {
    EXIT_PROGRAM = -1,
    DONE = 0,
    BOARD_UPDATE = 1,
    PARAMETER_UPDATE = 2,
    CMD_ERR = 3
};

/**
 * A command, includes description and pointer to handler function
 */
typedef struct _command {
    const char *command_name;
    command_arguments_description_t args;
    modes_availability_t modes_of_availability;

    enum command_status (*function)(sudoku_game_t *, const command_arguments_t *);
} command_t;

#endif

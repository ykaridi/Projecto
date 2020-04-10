#ifndef SUDOKU_COMMANDS_H
#define SUDOKU_COMMANDS_H

#include <stdio.h>

#include "../sudoku_game.h"

#define MAX_ARGS (3)

/**
 * An enum for command argument type
 */
enum argument_type {INTEGER = 0, STRING = 1, FLOAT = 2};

/**
 * A command argument description, includes type and range (for integers).
 * Used for validating validity of input and casting to the appropriate type.
 */
typedef struct _command_argument_description {
    enum argument_type argument_type;
    int optional;

    /* Only for numerical arguments */
    int enforce_range;
    /**
     * A union for the lower bound on the argument
     */
    union _lower_bound {
        int int_value;
        float float_value;
    } lower_bound;
    /**
     * A union for the upper bound on the argument
     */
    union _upper_bound {
        int int_value;
        float float_value;
    } upper_bound;
} command_argument_description_t;

/**
 * A struct representing a command argument list description.
 */
typedef struct _command_arguments_description {
    int num_arguments;
    command_argument_description_t* arguments[MAX_ARGS];
} command_arguments_description_t;

/**
 * A struct representing command argument, includes type and value (as a pointer)
 */
typedef struct _command_argument {
    enum argument_type type;
    int supplied;
    /**
     * A union for the argument value, over possible types
     */
    union value_t {
        char *str_value;
        int int_value;
        float float_value;
    } value;
} command_argument_t;

/**
 * A struct representing a list of arguments passed to a command.
 */
typedef struct _command_arguments {
    int num_arguments;
    command_argument_t arguments[MAX_ARGS];
} command_arguments_t;

/**
 * A struct containing the modes a command is available in
 */
typedef struct _modes_availability {
    int init;
    int edit;
    int solve;
} modes_availability_t;

/**
 * An enum for command execution status
 */
enum command_status {
    EXIT_PROGRAM = -1,
    DONE = 0,
    BOARD_UPDATE = 1,
    PARAMETER_UPDATE = 2,
    CMD_ERR = 3
};

/**
 * A struct representing a command, includes description and pointer to handler function
 */
typedef struct _command {
    const char *command_name;
    command_arguments_description_t args;
    modes_availability_t modes_of_availability;

    /**
     * Pointer to the command logic (function)
     * @return
     */
    enum command_status (*function)(sudoku_game_t *, const command_arguments_t *);
} command_t;

#endif

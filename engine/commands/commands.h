#ifndef SUDOKU_COMMANDS_H
#define SUDOKU_COMMANDS_H

#include <stdio.h>

#include "../sudoku_game.h"

#define MAX_ARGS (3)

#define DONE            (0)
#define RESTART_GAME    (1)
#define EXIT_PROGRAM    (2)

/**
 * Argument type, integer or string
 */
enum argument_type {INTEGER = 0, STRING = 1};

/**
 * A command argument description, includes type and range (for integers).
 * Used for validating validity of input and casting to the appropriate type.
 */
typedef struct _command_argument_description {
    enum argument_type argument_type;

    /* Only for integers */
    int enforce_integer_range;
    int lower_bound;
    int upper_bound;
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
    union value_t {
        char *str_value;
        int int_value;
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
 * A command output, includes operation to append to operations list (for undo and redo),
 * output to user and exit code.
 */
typedef struct _command_output {
    sudoku_game_operation_t *operation;
    char *output;

    int exit_code;
} command_output_t;

/**
 * Modes a command is available in
 */
typedef struct _modes_availability {
    int init;
    int edit;
    int solve;
} modes_availability_t;

/**
 * A command, includes description and pointer to handler function
 */
typedef struct _command {
    const char *command_name;
    const command_arguments_description_t args;
    const modes_availability_t modes_of_availability;

    command_output_t (*function)(sudoku_game_t *, command_arguments_t);
} command_t;

/**
 * A list of available commands
 */
typedef struct _command_list {
    int num_commands;
    command_t* *commands;
} command_list_t;

#endif

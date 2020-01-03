#ifndef SUDOKU_COMMANDS_H
#define SUDOKU_COMMANDS_H

#include <stdio.h>

#include "sudoku_game.h"

#define MAX_ARGS 3

/**
 * The command parsed by the parser.
 * x, y, z - The arguments to the command.
 */
typedef struct _command_args {
    int num_args;
    int args[MAX_ARGS];
} command_args_t;

/**
 * The output of a command.
 */
typedef struct _command_output {
    int success;
} command_output_t;

/**
 *  A command has a pointer to the function and a name of the function.
 */
typedef struct _command {
    command_output_t (*function)(sudoku_board_t *, command_args_t);
    char *command_name;
} command_t;


#define NUM_COMMANDS 5

command_t commands[NUM_COMMANDS];

#endif

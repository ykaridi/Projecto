#ifndef SUDOKU_COMMANDS_H
#define SUDOKU_COMMANDS_H

#include "sudoku_game.h"

#define MAX_ARGS 3

/*
 * The command parsed by the parser.
 * x, y, z - The arguments to the command.
 */
struct _command_args {
    int num_args;
    int args[MAX_ARGS];
} typedef command_args_t;

/*
 * The output of a command.
 */
struct _command_output {
    int success;
} typedef command_output_t;

/*
 *  A command has a pointer to the function and a name of the function.
 */
struct _command {
    command_output_t (*function)(command_args_t);

    char *command_name;
} typedef command_t;


#define NUM_COMMANDS 5

command_t commands[NUM_COMMANDS];

#endif

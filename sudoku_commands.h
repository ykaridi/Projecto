#ifndef SUDOKU_COMMANDS_H
#define SUDOKU_COMMANDS_H

#include <stdio.h>

#include "sudoku_game.h"
#include "sudoku_solver.h"

#define MAX_ARGS (3)

#define DONE            (0)
#define RESTART_GAME   (1)
#define EXIT_PROGRAM    (2)

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
    int exit_code;
} command_output_t;

/**
 *  A command has a pointer to the function and a name of the function.
 */
typedef struct _command {
    command_output_t (*function)(sudoku_game_t *, command_args_t);
    char *command_name;
} command_t;

command_output_t set(sudoku_game_t *game, command_args_t args);
command_output_t hint(sudoku_game_t *game, command_args_t args);
command_output_t validate(sudoku_game_t *game, command_args_t args);
command_output_t restart(sudoku_game_t *game, command_args_t args);
command_output_t exit_sudoku(sudoku_game_t *game, command_args_t args);

/**
 * A command list
 */
typedef struct _command_list {
    int num_commands;
    command_t *commands;
} command_list_t;

#endif

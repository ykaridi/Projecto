#ifndef PROJECTOHW3_SUDOKU_COMMANDS_H
#define PROJECTOHW3_SUDOKU_COMMANDS_H

#include "../sudoku_game.h"
#include "commands.h"

#define NUM_COMMANDS (17)

#define MAX_ITERATIONS (1000)

/**
 * A struct representing a list of available commands
 */
typedef struct _command_list {
    int num_commands;
    command_t commands[NUM_COMMANDS];

    struct _possible_arguments {
        command_argument_description_t generic_integer;
        command_argument_description_t generic_string;
        command_argument_description_t zo_float;
        command_argument_description_t boolean;
        command_argument_description_t optional_string;
        command_argument_description_t integer_boardidx_min0;
        command_argument_description_t integer_boardidx_min1;
        command_argument_description_t integer_boardsize_min0;
        command_argument_description_t integer_boardsize_min1;
    } possible_arguments;
} command_list_t;

void load_commands(command_list_t *command_list);
void update_arguments(command_list_t *command_list, const sudoku_game_t *game);

#endif

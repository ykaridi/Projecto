#ifndef PROJECTOHW3_USER_INTERFACE_H
#define PROJECTOHW3_USER_INTERFACE_H

#include "commands/commands.h"
#include "commands/parser.h"

void print_parsing_error(const parsing_errors_t *error, const command_t *command);
void print_game_operation(const sudoku_game_operation_t *operation, int undo);
void print_board(sudoku_board_t *board, int mark_errors, int mark_fixings);

#endif

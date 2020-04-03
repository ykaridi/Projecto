#ifndef MAIN_AUX_H
#define MAIN_AUX_H

#include "engine/sudoku_board.h"
#include "engine/commands/parser.h"

int num_length(int x, int base);
void print_repeated(char* s, int times);
void print_board(const sudoku_board_t *board);

void print_parsing_error(const parsing_errors_t *error, const command_t *command);

#endif

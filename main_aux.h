#ifndef MAIN_AUX_H
#define MAIN_AUX_H

#include "sudoku_game.h"

int num_length(int x, int base);
void print_repeated(char* s, int times);
void print_board(const sudoku_board_t *board);

int get_num_fixed_cells(sudoku_game_t *game);

#endif

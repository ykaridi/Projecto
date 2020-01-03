#ifndef MAIN_AUX_H
#define MAIN_AUX_H

#include "sudoku_game.h"

#define FIXED_CELLS_NUM 80

int num_length(int x, int base);
void print_repeated(char* s, int times);
void print_board(const sudoku_board_t *board);

#endif

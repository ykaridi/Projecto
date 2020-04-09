#ifndef PROJECTOHW3_RANDOM_H
#define PROJECTOHW3_RANDOM_H

#include "sudoku_board.h"

typedef struct _var_weight{
    int var;
    double weight;
} var_weight_t;

int choose_var(const var_weight_t* list, int len, double weight_sum);

int random_fill_empty_cells(sudoku_board_t *board, int x);

void keep_randomly_chosen_cells(sudoku_board_t* board, int y);

#endif

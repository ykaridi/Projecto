#ifndef SUDOKU_GRB_H
#define SUDOKU_GRB_H

#define LOG_FILE "grb.log"
#define DEFAULT_MODEL_NAME "sudoku_model"

#include "../sudoku_board.h"

#define GURUBI_OUTPUT 1

enum programming_type {
    ILP,
    LP
};

void solve_board(sudoku_board_t* board, enum programming_type type);

#endif /*SUDOKU_GRB_H */

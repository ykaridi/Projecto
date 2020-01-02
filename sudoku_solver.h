#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include "sudoku_game.h"

void generate_board(sudoku_board_t *board);
void solve_board(const sudoku_board_t *board, sudoku_board_t *solution);

#endif
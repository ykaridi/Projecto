#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include "sudoku_game.h"

void generate_board(sudoku_board *board);
void solve_board(const sudoku_board *board, sudoku_board *solution);

#endif
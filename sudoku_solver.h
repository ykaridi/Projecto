#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include "sudoku_game.h"

int deterministic_solution_inner(sudoku_board_t *board, int i, int j);
int randomized_solution_inner(sudoku_board_t *board, int i, int j);
int deterministic_solution(sudoku_board_t *board);
int randomized_solution(sudoku_board_t *board);

void randomize_board(sudoku_board_t *board, int num_cells);

#endif
#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include "sudoku_board.h"

int solve_board_inner(sudoku_board_t *board, int i, int j, int deterministic);
int solve_board(sudoku_board_t *board, int deterministic);

int update_solution(sudoku_game_t *game, int deterministic);
void reveal_cells(sudoku_game_t *game, int num_cells);
int reveal_cell(sudoku_game_t *game, int i, int j);

#endif

//
// Created by ykaridi on 19/03/2020.
//

#include "sudoku_board.h"
#include "data_structures/operation_list.h"

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

enum sudoku_mode {INIT = 0, EDIT = 1, SOLVE = 2};

typedef struct _sudoku_game {
    int rows;
    int cols;
    int total_rows;
    int total_cols;
    int sub_board_size;
    int total_size;

    int mark_errors;
    enum sudoku_mode mode;
    operation_node_t last_operation;

    sudoku_board_t *board;
    sudoku_board_t *temporary_board;
} sudoku_game_t;

#endif //SUDOKU_GAME_H

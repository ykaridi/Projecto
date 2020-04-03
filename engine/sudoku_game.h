#include "sudoku_board.h"
#include "data_structures/operation_list.h"

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

enum sudoku_mode {INIT = 0, EDIT = 1, SOLVE = 2};

typedef struct _sudoku_game {
    int mark_errors;
    enum sudoku_mode mode;
    operation_node_t last_operation;

    sudoku_board_t *board;
    sudoku_board_t *temporary_board;
} sudoku_game_t;

#endif //SUDOKU_GAME_H

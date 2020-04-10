#include "sudoku_board.h"
#include "rollback/operation_list.h"

#include "gurobi_c.h"


#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

enum sudoku_mode {INIT = 0, EDIT = 1, SOLVE = 2};

/**
 * A struct represnting a sudoku game
 */
typedef struct _sudoku_game {
    int mark_errors;
    enum sudoku_mode mode;
    operation_node_t *last_operation;

    sudoku_board_t *board;
    sudoku_board_t *temporary_board;
    GRBenv* grb_env;
} sudoku_game_t;

sudoku_game_t* create_game();
void load_board(sudoku_game_t *game, sudoku_board_t *board);
void destruct_game(sudoku_game_t *game);

#endif

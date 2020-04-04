#ifndef SUDOKU_GAME_OPERATION_H
#define SUDOKU_GAME_OPERATION_H

typedef struct _sudoku_game_operation sudoku_game_operation_t;

#include "operation_list.h"
#include "../sudoku_game.h"

enum sudoku_game_operation_type {
    HEAD = -2,
    ATOMIC = -1,
    SET = 0,
    AUTOFILL = 1,
    GENERATE = 2,
    GUESS = 3
};

struct _sudoku_game_operation {
    enum sudoku_game_operation_type operation_type;

    union _value {
        struct _atomic_operation {
            int row;
            int col;
            int new_value;
            int old_value;
        } atomic_operation;
        struct _composite_operation {
            operation_node_t *head;
            int arg1, arg2, arg3;
            float farg;
        } composite_operation;
    } value;
};

void destruct_game_operation(sudoku_game_operation_t *operation);

sudoku_game_operation_t *create_head_game_operation();
sudoku_game_operation_t *create_composite_game_operation(enum sudoku_game_operation_type operation_type, int arg1, int arg2, int arg3, float farg);
void append_atomic_operation(sudoku_game_operation_t *composite_operation, int row, int col, int old_value, int new_value);

void undo_operation(sudoku_game_t *game, sudoku_game_operation_t *operation);
void redo_operation(sudoku_game_t *game, sudoku_game_operation_t *operation);

#endif

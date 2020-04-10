#ifndef SUDOKU_GAME_OPERATION_H
#define SUDOKU_GAME_OPERATION_H

typedef struct _sudoku_game_operation sudoku_game_operation_t;

#include "operation_list.h"
#include "../sudoku_game.h"

/**
 * An enum for the different game operation types
 * HEAD represents the start of an operation list (not an actual operation)
 * ATOMIC represents a single change to a cell in the board caused by a command
 */
enum sudoku_game_operation_type {
    HEAD = -2,
    ATOMIC = -1,
    SET = 0,
    AUTOFILL = 1,
    GENERATE = 2,
    GUESS = 3
};

/**
 * A struct for a sudoku game operation
 * An atomic operation is changing a single cell in the board
 * A composite operation is a command given by the user, comprised of (perhaps many) atomic operations.
 */
struct _sudoku_game_operation {
    enum sudoku_game_operation_type operation_type;

    /**
     * A union for the game operation value,
     * Depends on being atomic of composite
     */
    union _value {
        /**
         * A struct for describing an atomic operation
         */
        struct _atomic_operation {
            int row;
            int col;
            int new_value;
            int old_value;
        } atomic_operation;
        /**
         * A struct for describing a composite operation
         */
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

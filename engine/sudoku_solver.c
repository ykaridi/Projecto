#include <stdio.h>
#include <stdlib.h>
#include "sudoku_solver.h"
#include "sudoku_board.h"
#include "data_structures/stack.h"

int backtracking(sudoku_board_t *board) {
    int idx = 0, row = 0, col = 0, num_sols = 0;
    stack_t *stack;

    if (!check_board(board)) {
        return 0;
    }

    stack = create_stack();
    stack_push(stack, 0);
    while (!stack_empty(stack)) {
        idx = stack_peek(stack);
        if (idx >= board->total_size) {
            num_sols += 1;
            stack_pop(stack);
            continue;
        }

        row = idx / board->total_rows, col = idx % board->total_rows;
        if (get_cell_metadata_flattened(board, row, col) == EMPTY_CELL)
            set_cell_metadata_flattened(board, TEMPORARY_METADATA, row, col);
        if (get_cell_metadata_flattened(board, row, col) != TEMPORARY_METADATA) {
            stack_pop(stack);
            stack_push(stack, idx + 1);
            continue;
        }
        set_cell_flattened(board, row, col, get_cell_flattened(board, row, col) + 1);

        if (get_cell_flattened(board, row, col) > board->sub_board_size) {
            set_cell_flattened(board, row, col, EMPTY_CELL);
            stack_pop(stack);
            continue;
        }

        if (!check_value(board, 0, row, col))
            continue;
        stack_push(stack, idx + 1);
    }

    destroy_stack(stack);
    return num_sols;
}
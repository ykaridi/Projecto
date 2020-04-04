#include "sudoku_solver.h"
#include "../sudoku_board.h"
#include "stack.h"

int backtracking(sudoku_board_t *board) {
    int idx = 0, row = 0, col = 0, num_sols = 0, v;
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

        row = idx / board->total_cols, col = idx % board->total_rows;
        if (get_cell_flattened(board, row, col) == EMPTY_CELL)
            set_cell_metadata_flattened(board, TEMPORARY_METADATA, row, col);
        if (get_cell_metadata_flattened(board, row, col) != TEMPORARY_METADATA) {
            stack_pop(stack);
            stack_push(stack, idx + 1);
            continue;
        }
        v = get_cell_flattened(board, row, col) + 1;
        set_cell_flattened(board, v, row, col);

        if (v > board->sub_board_size) {
            set_cell_flattened(board, EMPTY_CELL, row, col);
            set_cell_metadata_flattened(board, EMPTY_METADATA, row, col);
            stack_pop(stack);
            continue;
        }

        /* TODO: REPLACE WITH BETTER CHECK? */
        if (!check_value(board, 0, row, col))
            continue;
        stack_push(stack, idx + 1);
    }

    destroy_stack(stack);
    return num_sols;
}

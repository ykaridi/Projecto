#include "sudoku_solver.h"
#include "../sudoku_board.h"
#include "backtracking_stack.h"

int backtracking(sudoku_board_t *board) {
    int idx = 0, row = 0, col = 0, num_sols = 0, v;
    backtracking_stack_t *stack;

    if (!check_board(board)) {
        return 0;
    }

    stack = create_backtracking_stack();
    backtracking_stack_push(stack, 0);
    while (!backtracking_stack_empty(stack)) {
        idx = backtracking_stack_peek(stack);
        if (idx >= board->total_size) {
            num_sols += 1;
            backtracking_stack_pop(stack);
            continue;
        }

        row = idx / board->total_cols, col = idx % board->total_rows;
        if (get_cell_flattened(board, row, col) == EMPTY_CELL)
            set_cell_metadata_flattened(board, TEMPORARY_METADATA, row, col);
        if (get_cell_metadata_flattened(board, row, col) != TEMPORARY_METADATA) {
            backtracking_stack_pop(stack);
            backtracking_stack_push(stack, idx + 1);
            continue;
        }
        v = get_cell_flattened(board, row, col) + 1;
        set_cell_flattened(board, v, row, col);

        if (v > board->sub_board_size) {
            set_cell_flattened(board, EMPTY_CELL, row, col);
            set_cell_metadata_flattened(board, EMPTY_METADATA, row, col);
            backtracking_stack_pop(stack);
            continue;
        }

        if (!check_value_flattened(board, v, row, col))
            continue;
        backtracking_stack_push(stack, idx + 1);
    }

    destroy_backtracking_stack(stack);
    return num_sols;
}

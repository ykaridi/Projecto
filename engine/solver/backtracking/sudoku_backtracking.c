#include "sudoku_backtracking.h"
#include "../../sudoku_board.h"
#include "backtracking_stack.h"

/**
 * Performs exhaustive backtracking to count the amount of solutions a sudoku board has
 * @param board
 * @return
 */
int backtracking(sudoku_board_t *board) {
    int idx = 0, row = 0, col = 0, num_sols = 0, v;
    backtracking_stack_t *stack;

    /* Check if board is erroneous to begin with */
    if (!check_board(board)) {
        return 0;
    }

    /* Create stack for simulating recursion */
    stack = create_backtracking_stack();
    backtracking_stack_push(stack, 0);
    while (!backtracking_stack_empty(stack)) {
        /* Check if we completed the whole board */
        idx = backtracking_stack_peek(stack);
        if (idx >= board->total_size) {
            num_sols += 1;
            backtracking_stack_pop(stack);
            continue;
        }

        /* Check if cell already contains a (non-backtracking related) value */
        row = idx / board->total_cols, col = idx % board->total_rows;
        if (get_cell_flattened(board, row, col) == EMPTY_CELL)
            set_cell_metadata_flattened(board, TEMPORARY_METADATA, row, col);
        if (get_cell_metadata_flattened(board, row, col) != TEMPORARY_METADATA) {
            /* Skip this cell, and move on to the next one */
            backtracking_stack_pop(stack);
            backtracking_stack_push(stack, idx + 1);
            continue;
        }

        /* Check next value for the current cell */
        v = get_cell_flattened(board, row, col) + 1;
        set_cell_flattened(board, v, row, col);
        /* Check if current cell exceeded maximal value */
        if (v > board->sub_board_size) {
            set_cell_flattened(board, EMPTY_CELL, row, col);
            set_cell_metadata_flattened(board, EMPTY_METADATA, row, col);
            backtracking_stack_pop(stack);
            continue;
        }

        /* Check if current value causes is valid */
        if (!check_value_flattened(board, v, row, col))
            continue;
        /* Continue to next cell */
        backtracking_stack_push(stack, idx + 1);
    }

    destroy_backtracking_stack(stack);
    return num_sols;
}

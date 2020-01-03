#include <stdio.h>
#include <stdlib.h>
#include "sudoku_solver.h"
#include "sudoku_game.h"

#define MAX_NUMBER (9)

/**
 * Runs a deterministic backtracking algorithm
 * @param board Sudoku board object
 * @param i Current i index
 * @param j Current j index
 * @return Boolean success flag
 */
int deterministic_solution_inner(sudoku_board_t *board, int i, int j) {
    int value = 0;
    char metadata;

    if (j >= board->total_cols) {
        i += j / board->total_cols;
        j %= board->total_cols;
    }
    if (i >= board->total_rows) {
        return TRUE;
    }

    metadata = get_cell_metadata_flattened(board, i, j);
    if (metadata == FIXED_METADATA)
        return deterministic_solution_inner(board, i, j + 1);

    for (value = 1; value <= board->sub_board_size; value++) {
        if (check_value(board, value, i, j))
            continue;

        set_cell_flattened(board, value, i, j);
        set_cell_metadata_flattened(board, TEMPORARY_METADATA, i, j);
        if (deterministic_solution_inner(board, i, j + 1))
            return TRUE;

        set_cell_flattened(board, EMPTY_CELL, i, j);
        set_cell_metadata_flattened(board, EMPTY_METADATA, i, j);
    }

    return FALSE;
}
/**
 * Runs a deterministic backtracking algorithm
 * @param board Sudoku board object
 * @return Boolean success flag
 */
int deterministic_solution(sudoku_board_t *board) {
    return deterministic_solution_inner(board, 0, 0);
}
/**
 * Runs a randomized backtracking algorithm
 * VERY IMPORTANT: WORKS ONLY WITH BOARDS WITH VALUES <= 9
 * @param board Sudoku board object
 * @param i Current i index
 * @param j Current j index
 * @return Boolean success flag
 */
int randomized_solution_inner(sudoku_board_t *board, int i, int j) {
    int value, selection, num_valid_values, curr_selection = 0;
    int legal_map[MAX_NUMBER + 1];
    char metadata;

    if (j >= board->total_cols) {
        i += j / board->total_cols;
        j %= board->total_cols;
    }
    if (i >= board->total_rows) {
        return TRUE;
    }

    metadata = get_cell_metadata_flattened(board, i, j);
    if (metadata == FIXED_METADATA)
        return randomized_solution_inner(board, i, j + 1);

    legal_map[0] = FALSE;
    num_valid_values = 0;
    for(value = 1; value <= MAX_NUMBER; value++) {
        if (check_value(board, value, i, j)) {
            legal_map[value] = FALSE;
        } else {
            num_valid_values++;
            legal_map[value] = TRUE;
        }
    }

    while (num_valid_values > 0) {
        if (num_valid_values == 1) {
            selection = 0;
        } else {
            selection = rand() % num_valid_values;
        }

        curr_selection = 0;
        value = 0;
        while (curr_selection <= selection) {
            value += 1;
            if (legal_map[value] == TRUE) {
                curr_selection += 1;
            }
        }

        set_cell_flattened(board, value, i, j);
        set_cell_metadata_flattened(board, TEMPORARY_METADATA, i, j);
        if (randomized_solution_inner(board, i, j + 1) == TRUE) {
            return TRUE;
        }

        legal_map[value] = FALSE;
        set_cell_flattened(board, EMPTY_CELL, i, j);
        set_cell_metadata_flattened(board, EMPTY_METADATA, i, j);
        num_valid_values--;
    }

    return FALSE;
}
/**
 * Runs a randomized backtracking algorithm
 * VERY IMPORTANT: WORKS ONLY WITH BOARDS WITH VALUES <= 9
 * @param board Sudoku board object
 * @return Boolean success flag
 */
int randomized_solution(sudoku_board_t *board) {
    return randomized_solution_inner(board, 0, 0);
}

/**
 * Randomizes the remaining cells in the board
 * @param board Sudoku board object
 * @param num_cells Number of cells to randomize
 */
void randomize_board(sudoku_board_t *board, int num_cells) {
    int X, Y = 0;
    randomized_solution(board);

    while (num_cells > 0) {
        X = rand() % board->total_rows;
        Y = rand() % board->total_cols;

        if (get_cell_metadata_flattened(board, Y, X) == TEMPORARY_METADATA) {
            set_cell_metadata_flattened(board, FIXED_METADATA, Y, X);
            num_cells -= 1;
        }
    }

    clear_board_temps(board);
}

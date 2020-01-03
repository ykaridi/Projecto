#include <stdio.h>
#include <stdlib.h>
#include "sudoku_solver.h"
#include "sudoku_game.h"

#define CHECK_VALUE (!check_row(board, i, value) || !check_column(board, j, value) ||\
                    !check_sub_board(board, SUB_BOARD_CONVERSION, value))
#define MAX_NUMBER (9)

/// Runs a deterministic backtracking algorithm
/// \param board Sudoku board object
/// \param i Current i index
/// \param j Current j index
/// \return Boolean success flag
int deterministic_solution_inner(sudoku_board_t *board, int i, int j) {
    if (j >= board->total_cols) {
        i += j / board->total_cols;
        j %= board->total_cols;
    }
    if (i >= board->total_rows) {
        return TRUE;
    }

    char metadata = get_cell_metadata_flattened(board, i, j);
    if (metadata == FIXED_METADATA)
        return deterministic_solution_inner(board, i, j + 1);

    for (int value = 1; value <= board->sub_board_size; value++) {
        if (CHECK_VALUE)
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
/// Runs a deterministic backtracking algorithm
/// \param board Sudoku board object
/// \return Boolean success flag
int deterministic_solution(sudoku_board_t *board) {
    return deterministic_solution_inner(board, 0, 0);
}
/// Runs a randomized backtracking algorithm
/// VERY IMPORTANT: WORKS ONLY WITH BOARDS WITH VALUES <= 9
/// \param board Sudoku board object
/// \param i Current i index
/// \param j Current j index
/// \return Boolean success flag
int randomized_solution_inner(sudoku_board_t *board, int i, int j) {
    if (j >= board->total_cols) {
        i += j / board->total_cols;
        j %= board->total_cols;
    }
    if (i >= board->total_rows) {
        return TRUE;
    }

    char metadata = get_cell_metadata_flattened(board, i, j);
    if (metadata == FIXED_METADATA)
        return randomized_solution_inner(board, i, j + 1);

    int legal_map[MAX_NUMBER + 1];
    legal_map[0] = FALSE;
    int num_valid_values = 0;
    for(int value = 1; value <= MAX_NUMBER; value++) {
        if (CHECK_VALUE) {
            legal_map[value] = FALSE;
        } else {
            num_valid_values++;
            legal_map[value] = TRUE;
        }
    }

    int selection = 0;
    while (num_valid_values > 0) {
        if (num_valid_values == 1) {
            selection = 0;
        } else {
            selection = rand() % num_valid_values;
        }

        int curr_selection = 0;
        int value = 1;
        while (curr_selection <= selection) {
            if (legal_map[value] == TRUE) {
                curr_selection += 1;
            }
            value += 1;
        }
        value -= 1;

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
/// Runs a randomized backtracking algorithm
/// VERY IMPORTANT: WORKS ONLY WITH BOARDS WITH VALUES <= 9
/// \param board Sudoku board object
/// \return Boolean success flag
int randomized_solution(sudoku_board_t *board) {
    return randomized_solution_inner(board, 0, 0);
}

/// Randomizes the remaining cells in the board
/// \param board Sudoku board object
void randomize_board(sudoku_board_t *board, int num_cells) {
    randomized_solution(board);

    while (num_cells > 0) {
        int X = rand() % board->total_rows;
        int Y = rand() % board->total_cols;

        if (get_cell_metadata_flattened(board, X, Y) == TEMPORARY_METADATA) {
            set_cell_metadata_flattened(board, FIXED_METADATA, X, Y);
            num_cells -= 1;
        }
    }

    //clear_board_temps(board);
}
#include <stdio.h>
#include <stdlib.h>
#include "sudoku_solver.h"
#include "sudoku_board.h"

#define MAX_NUMBER (9)

/**
 * Attempts solving the board using (deterministic or randomized) backtracking
 * @param board Sudoku board object
 * @param i Current i
 * @param j Current j
 * @param deterministic Boolean flag indicating whether to solve deterministically or randomly (TRUE for deterministic)
 * @return
 */
int solve_board_inner(sudoku_board_t *board, int i, int j, int deterministic) {
    int value, selection, num_valid_values, curr_selection = 0;
    int legal_map[MAX_NUMBER + 1];

    if (j >= board->total_cols) {
        i += j / board->total_cols;
        j %= board->total_cols;
    }
    if (i >= board->total_rows) {
        return TRUE;
    }

    if (get_cell_flattened(board, i, j) != EMPTY_CELL)
        return solve_board_inner(board, i, j + 1, deterministic);

    legal_map[0] = FALSE;
    num_valid_values = 0;
    for(value = 1; value <= MAX_NUMBER; value++) {
        if (!check_value(board, value, i, j)) {
            legal_map[value] = FALSE;
        } else {
            num_valid_values++;
            legal_map[value] = TRUE;
        }
    }

    while (num_valid_values > 0) {
        if (num_valid_values == 1 || deterministic == TRUE) {
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
        if (solve_board_inner(board, i, j + 1, deterministic) == TRUE) {
            return TRUE;
        }

        legal_map[value] = FALSE;
        set_cell_flattened(board, EMPTY_CELL, i, j);
        set_cell_metadata_flattened(board, EMPTY_METADATA, i, j);
        num_valid_values--;
    }

    return FALSE;
}

int solve_board(sudoku_board_t *board, int deterministic) {
    return solve_board_inner(board, 0, 0, deterministic);
}

/**
 * Updates the game's solution board
 * @param game Sudoku game object
 * @param deterministic Boolean flag indicating whether to solve deterministically or randomly (TRUE for deterministic)
 * @returns Boolean flag indicating if board is solvable (YES=TRUE, NO=FALSE)
 */
int update_solution(sudoku_game_t *game, int deterministic) {
    int result;

    force_clear_board(game->temporary_board);
    copy_board(game->board, game->temporary_board);

    result = solve_board(game->temporary_board, deterministic);
    if (result == TRUE) {
        force_clear_board(game->solved_board);
        copy_board(game->temporary_board, game->solved_board);
    }

    return result;
}

/**
 * Reveals cells from the current game solution
 * @param game Sudoku game object
 * @param num_cells Number of cells to reveal (reveals only cells marked as temp)
 */
void reveal_cells(sudoku_game_t *game, int num_cells) {
    int X, Y;

    while (num_cells > 0) {
        X = rand() % game->board->total_rows;
        Y = rand() % game->board->total_cols;

        if (reveal_cell(game, Y, X))
            num_cells -= 1;
    }
}

/**
 * Reveals a single cell from the current game solution
 * @param game Sudoku game object
 * @param i
 * @param j
 * @returns Boolean success flag
 */
int reveal_cell(sudoku_game_t *game, int i, int j) {
    if (get_cell_metadata_flattened(game->board, i, j) != FIXED_METADATA) {
        int value = get_cell_flattened(game->solved_board, i, j);
        set_cell_flattened(game->board, value, i, j);
        set_cell_metadata_flattened(game->board, FIXED_METADATA, i, j);
        return TRUE;
    }

    return FALSE;
}

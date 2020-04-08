#include <stdlib.h>
#include <string.h>

#include "random.h"
#include "../utils.h"
#include "user_interface.h"

typedef struct _cell {
    int row;
    int col;
} cell_t;

/**
 * Returns x==y.
 * @param x
 * @param y
 * @return
 */
int cell_cmp(cell_t x, cell_t y) {
    return x.row == y.row && x.col == y.col;
}

/**
 * Gets a board and returns a cell_t list containing the empty and legal cells of board
 * @param board
 * @param cells The array of the empty cells.
 * @param len   After the function is
 */
void get_empty_cells(sudoku_board_t *board, cell_t *cells, int *len) {
    int i, j;

    *len = 0;
    for (i = 0; i < board->total_rows; ++i) {
        for (j = 0; j < board->total_cols; ++j) {
            if (get_cell_flattened(board, i, j) == 0) {
                cells[*len].row = i;
                cells[*len].col = j;
                (*len)++;
            }
        }
    }
}

/**
 * Choose a random legal value for cell. When calling this function to get the legal
 * values, destroy_board should be set to FALSE.
 * After all uses of this function, call it with cleanup set to TRUE.
 * @param board
 * @param cell
 * @param cleanup
 * @return
 */
int choose_random_legal_value(sudoku_board_t *board, cell_t cell, int cleanup) {
    int len = 0, i = cell.row, j = cell.col, k;
    static int *legal_values = NULL;

    if (cleanup) {
        free(legal_values);
        legal_values = NULL;
        return 0;
    }

    if (legal_values == NULL) {
        legal_values = malloc(board->sub_board_size * sizeof(int));
        if (!legal_values) {
            EXIT_ON_ERROR("malloc")
        }
    }

    for (k = 0; k < board->sub_board_size; ++k) {
        if (check_value_flattened(board, k + 1, i, j)) {
            legal_values[len++] = k + 1;
        }
    }
    if (len == 0) {
        return 0;
    }

    if (len == 1) {
        return legal_values[0];
    } else {
        return legal_values[rand() % len];
    }
}


/**
 * Randomly fill x empty cells of board.
 * @param board
 * @param x
 * @return SUCCESS/ERROR.
 */
int random_fill_empty_cells(sudoku_board_t *board, int x) {
    cell_t *empty_cells;
    int empty_cell_num, index;
    cell_t chosen_cell, empty_cell = {0, 0};

    empty_cells = malloc(board->total_size * sizeof(cell_t));
    if (!empty_cells) {
        free(empty_cells);
        EXIT_ON_ERROR("malloc")
    }

    get_empty_cells(board, empty_cells, &empty_cell_num);
    for (; x > 0; x--) {
        int value;
        /* Not enough legal cells. */
        if (empty_cell_num < x) {
            free(empty_cells);
            choose_random_legal_value(NULL, empty_cell, TRUE);
            return ERROR;
        }

        /* Choose a random empty cell */
        do {
            index = rand() % empty_cell_num;
            chosen_cell = empty_cells[index];
        } while (cell_cmp(chosen_cell, empty_cell));

        value = choose_random_legal_value(board, chosen_cell, FALSE);
        /* No legal value for chosen cell */
        if (value == 0) {
            free(empty_cells);
            choose_random_legal_value(NULL, chosen_cell, TRUE);
            return ERROR;
        }

        set_cell_flattened(board, value, chosen_cell.row, chosen_cell.col);
        empty_cells[index] = empty_cell;
    }

    free(empty_cells);
    choose_random_legal_value(NULL, empty_cell, TRUE);
    return SUCCESS;
}

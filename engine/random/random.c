#include <stdlib.h>

#include "random.h"
#include "../../utils.h"
#include "random_value_chooser.h"


typedef struct _cell {
    int row;
    int col;
} cell_t;

/**
 * Returns x==y.
 */
int cell_cmp(cell_t x, cell_t y) {
    return x.row == y.row && x.col == y.col;
}

/**
 * Gets a board and returns a cell_t list containing the empty cells of board
 * @param board
 * @param cells The array of the empty cells.
 * @param len   After the function is number of empty cells.
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
 * Randomly fill x empty cells of board.
 * @param board
 * @param x
 * @return SUCCESS/ERROR.
 */
int random_fill_empty_cells(sudoku_board_t *board, int x) {
    cell_t *empty_cells;
    int empty_cell_num, index;
    rand_value_chooser_t* chooser;
    cell_t chosen_cell, empty_cell = {0, 0};


    empty_cells = malloc(board->total_size * sizeof(cell_t));
    if (!empty_cells) {
        free(empty_cells);
        EXIT_ON_ERROR("malloc")
    }

    get_empty_cells(board, empty_cells, &empty_cell_num);
    chooser = create_chooser(board->sub_board_size);
    for (; x > 0; x--) {
        int value;
        /* Not enough legal cells. */
        if (empty_cell_num < x) {
            free(empty_cells);
            destruct_chooser(chooser);
            return ERROR;
        }

        /* Choose a random empty cell */
        do {
            index = rand() % empty_cell_num;
            chosen_cell = empty_cells[index];
        } while (cell_cmp(chosen_cell, empty_cell));

        value = choose_random_legal_value(chooser, board, chosen_cell.row, chosen_cell.col);
        /* No legal value for chosen cell */
        if (value == 0) {
            free(empty_cells);
            destruct_chooser(chooser);
            return ERROR;
        }

        set_cell_flattened(board, value, chosen_cell.row, chosen_cell.col);
        empty_cells[index] = empty_cell;
    }

    free(empty_cells);
    destruct_chooser(chooser);
    return SUCCESS;
}

/**
 * Delete all but y cells in the board.
 */
void keep_randomly_chosen_cells(sudoku_board_t *board, int y) {
    int k = 0, row, col;

    while (k < board->total_size - y) {
        int index = rand() % board->total_size;
        row = index / board->total_cols;
        col = index % board->total_rows;

        if (get_cell_flattened(board, row, col) == EMPTY_CELL) {
            continue;
        }
        set_cell_flattened(board, EMPTY_CELL, row, col);
        set_cell_metadata_flattened(board, EMPTY_METADATA, row, col);
        k++;
    }
}

/**
 * Choose randomly a variable according to its weight.
 */
int choose_var(const var_weight_t *list, int len, double weight_sum) {
    double rand_val = ((double) rand() / RAND_MAX) * weight_sum;
    double temp_sum = 0;
    int i;
    for (i = 0; i < len; ++i) {
        if (rand_val < temp_sum) {
            return list[i].var;
        }
        temp_sum += list[i].weight;
    }

    return list[len - 1].var;
}

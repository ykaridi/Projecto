#include "sudoku_board.h"
#include "../utils.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Create a sudoku board object
 * @param
 * @param
 * @return Pointer to the board object
 */
sudoku_board_t *create_board(int rows, int cols) {
    sudoku_board_t *board;

    board = malloc(sizeof(sudoku_board_t));
    if (board == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    board->rows = rows;
    board->cols = cols;
    board->total_rows = board -> total_cols = rows * cols;
    board->sub_board_size = rows * cols;
    board->total_size = board->total_rows * board->total_cols;

    board->board = malloc(sizeof(int) * rows * rows * cols * cols);
    board->cell_metadata = malloc(sizeof(char) * rows * rows * cols * cols);
    /* Check if malloc was successful */
    if (board->board == NULL || board->cell_metadata == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    /* Clear board */
    force_clear_board(board);
    return board;
}

/**
 * Destroy (free) a sudoku board object
 * @param board
 */
void destruct_board(sudoku_board_t *board) {
    free(board->board);
    free(board->cell_metadata);
    free(board);
}

/**
 * Clears board of temporary cells
 * @param board
 */
void clear_board_temps(sudoku_board_t *board) {
    int i, j = 0;

    for (i = 0; i < board->total_rows; i++) {
        for (j = 0; j < board->total_cols; j++) {
            if (get_cell_metadata_flattened(board, i, j) == TEMPORARY_METADATA) {
                set_cell_flattened(board, EMPTY_CELL, i, j);
                set_cell_metadata_flattened(board, EMPTY_METADATA, i, j);
            }
        }
    }
}

/**
 * Clears all metadata of the board
 * @param board
 */
void clear_board_metadata(sudoku_board_t *board) {
    int i, j = 0;

    for (i = 0; i < board->total_rows; i++) {
        for (j = 0; j < board->total_cols; j++) {
            set_cell_metadata_flattened(board, EMPTY_METADATA, i, j);
        }
    }
}

/**
 * Clears all board, including metadata
 * @param board
 */
void force_clear_board(sudoku_board_t *board) {
    int i = 0;
    for (i = 0; i < board->total_rows * board->total_cols; i++) {
        board->board[i] = EMPTY_CELL;
        board->cell_metadata[i] = EMPTY_METADATA;
    }
}

/**
 * Copies a board (Assuming both boards are of same size!)
 * @param board_in
 * @param board_out
 */
void copy_board(sudoku_board_t *board_in, sudoku_board_t *board_out) {
    int i, j = 0;
    for (i = 0; i < board_in->total_rows; i++) {
        for(j = 0; j < board_in->total_cols; j++) {
            set_cell_flattened(board_out, get_cell_flattened(board_in, i, j), i, j);
            set_cell_metadata_flattened(board_out, get_cell_metadata_flattened(board_in, i, j), i, j);
        }
    }
}

/**
 * Checks if an index is valid
 * @param board Sudoku board object
 * @param sub_board_i
 * @param sub_board_j
 * @param inner_i
 * @param inner_j
 * @return 1 = In board, 0 = Invalid (not in board)
 */
int is_in_board(const sudoku_board_t *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    return in_range(sub_board_i, 0, board->cols) && in_range(sub_board_j, 0, board->rows) &&
           in_range(inner_i, 0, board->rows) && in_range(inner_j, 0, board->cols);
}

/**
 * Checks if an index is valid
 * @param board
 * @param i
 * @param j
 * @return 1 = In board, 0 = Invalid (not in board)
 */
int is_in_board_flattened(const sudoku_board_t *board, int i, int j) {
    return is_in_board(board, FLAT_CONVERSION);
}

/**
 * Returns a cell's value
 * @param board
 * @param sub_board_i
 * @param sub_board_j
 * @param inner_i
 * @param inner_j
 * @return Cell value (or ERROR on error)
 */
int get_cell(const sudoku_board_t *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    if (!is_in_board(board, sub_board_i, sub_board_j, inner_i, inner_j))
        return ERROR;
    return board->board[ARR_CONVERSION];
}

/**
 * Returns a cell's value
 * @param board
 * @param i
 * @param j
 * @return Cell value (or ERROR on error)
 */
int get_cell_flattened(const sudoku_board_t *board, int i, int j) {
    return get_cell(board, FLAT_CONVERSION);
}

/**
 * Sets a cell's value
 * @param board
 * @param value Cell's new value
 * @param sub_board_i
 * @param sub_board_j
 * @param inner_i
 * @param inner_j
 * @return Success flag
 */
int set_cell(sudoku_board_t *board, int value, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    if (!is_in_board(board, sub_board_i, sub_board_j, inner_i, inner_j))
        return ERROR;

    board->board[ARR_CONVERSION] = value;
    return SUCCESS;
}

/**
 * Sets a cell's value
 * @param board
 * @param value Cell's new value
 * @param i
 * @param j
 * @return Success flag
 */
int set_cell_flattened(sudoku_board_t *board, int value, int i, int j) {
    return set_cell(board, value, FLAT_CONVERSION);
}

/**
 * Returns a cell's metadata
 * @param board
 * @param sub_board_i
 * @param sub_board_j
 * @param inner_i
 * @param inner_j
 * @return Cell metadata (or ERROR on error)
 */
char get_cell_metadata(const sudoku_board_t *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    if (!is_in_board(board, sub_board_i, sub_board_j, inner_i, inner_j))
        return ERROR;
    return board->cell_metadata[ARR_CONVERSION];
}

/**
 * Returns a cell's metadata
 * @param board
 * @param i
 * @param j
 * @return Cell metadata (or ERROR on error)
 */
char get_cell_metadata_flattened(const sudoku_board_t *board, int i, int j) {
    return get_cell_metadata(board, FLAT_CONVERSION);
}

/**
 * Sets a cell's metadata
 * @param board
 * @param metadata Cell's new metadata
 * @param sub_board_i
 * @param sub_board_j
 * @param inner_i
 * @param inner_j
 * @return Success flag
 */
int
set_cell_metadata(sudoku_board_t *board, char metadata, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    if (!is_in_board(board, sub_board_i, sub_board_j, inner_i, inner_j))
        return ERROR;

    board->cell_metadata[ARR_CONVERSION] = metadata;
    return 0;
}

/**
 * Sets a cell's metadata
 * @param board
 * @param metadata Cell's new metadata
 * @param i
 * @param j
 * @return
 */
int set_cell_metadata_flattened(sudoku_board_t *board, char metadata, int i, int j) {
    return set_cell_metadata(board, metadata, FLAT_CONVERSION);
}

/**
 * Checks if row is currently legal, if value > 0 checks that it doesn't already exist in row
 * @param board Sudoku board object
 * @param row
 * @param value
 * @return Boolean flag
 */
int check_row(const sudoku_board_t *board, int row, int value) {
    int col1, col2 = 0;

    for (col1 = 0; col1 < board->total_cols; col1++) {
        int current_cell = get_cell_flattened(board, row, col1);
        if (current_cell == 0)
            continue;

        if (value > 0) {
            /* Check if value was found in row */
            if (current_cell == value)
                return FALSE;
        } else {
            /* Check if some other cell has a colliding value */
            for (col2 = col1 + 1; col2 < board->total_cols; col2++) {
                if (current_cell == get_cell_flattened(board, row, col2))
                    return FALSE;
            }
        }
    }

    return TRUE;
}

/**
 * Checks if column is currently legal, if value > 0 checks that it doesn't already exist in column
 * @param board Sudoku board object
 * @param col
 * @param value
 * @return Boolean flag
 */
int check_column(const sudoku_board_t *board, int col, int value) {
    int row1, row2 = 0;

    for (row1 = 0; row1 < board->total_rows; row1++) {
        int current_cell = get_cell_flattened(board, row1, col);
        if (current_cell == 0)
            continue;

        if (value > 0) {
            /* Check if value was found in column */
            if (current_cell == value)
                return FALSE;
        } else {
            /* Check if some other cell has a colliding value */
            for (row2 = row1 + 1; row2 < board->total_rows; row2++) {
                if (current_cell == get_cell_flattened(board, row2, col))
                    return FALSE;
            }
        }
    }

    return TRUE;
}

/**
 * Checks if sub board is currently legal, if value > 0 checks that it doesn't already exist in sub board
 * @param board Sudoku board object
 * @param sub_board_i
 * @param sub_board_j
 * @param value
 * @return Boolean flag
 */
int check_sub_board(const sudoku_board_t *board, int sub_board_i, int sub_board_j, int value) {
    int inner_i1, inner_j1, inner_i2, inner_j2 = 0;
    for (inner_i1 = 0; inner_i1 < board->rows; inner_i1++) {
        for (inner_j1 = 0; inner_j1 < board->cols; inner_j1++) {
            int current_cell = get_cell(board, sub_board_i, sub_board_j, inner_i1, inner_j1);
            if (current_cell == EMPTY_CELL)
                continue;

            if (value > 0) {
                /* Check if value was found in sub board */
                if (current_cell == value)
                    return FALSE;
            } else {
                /* Check if some other cell has a colliding value */
                for (inner_i2 = 0; inner_i2 < board->rows; inner_i2++) {
                    for (inner_j2 = 0; inner_j2 < board->cols; inner_j2++) {
                        if (inner_i1 == inner_i2 && inner_j1 == inner_j2)
                            continue;

                        if (get_cell(board, sub_board_i, sub_board_j, inner_i2, inner_j2) == current_cell)
                            return FALSE;
                    }
                }
            }
        }
    }

    return TRUE;
}

/**
 * Check if board contains an error
 * @param board
 * @return
 */
int check_board(const sudoku_board_t *board) {
    int i, j = 0;

    /* Iterate over all row, columns and sub boards and check them for errors */
    for (i = 0; i < board->total_rows; i++) {
        if (check_row(board, i, 0) == FALSE)
            return FALSE;
    }
    for (j = 0; j < board->total_cols; j++) {
        if (check_column(board, j, 0) == FALSE)
            return FALSE;
    }
    for (i = 0; i < board->cols; i++) {
        for (j = 0; j < board->rows; j++) {
            if (check_sub_board(board, i, j, 0) == FALSE)
                return FALSE;
        }
    }

    return TRUE;
}


/**
 * Checks if value is legal
 * @param board
 * @param value
 * @param i
 * @param j
 * @return
 */
int check_value_flattened(sudoku_board_t *board, int value, int i, int j) {
    int old_value = get_cell_flattened(board, i, j), ret = 0;
    /* Temporarily empty cell */
    if (value != 0)
        set_cell_flattened(board, 0, i, j);

    /* Check the appropriate row, column and sub board */
    ret = check_row(board, i, value)
            && check_column(board, j, value)
            && check_sub_board(board, SUB_BOARD_CONVERSION, value);

    /* Restore cell value */
    if (value != 0)
        set_cell_flattened(board, old_value, i, j);
    return ret;
}

/**
 * Checks if value is legal
 * @param board
 * @param value
 * @param sub_board_i
 * @param sub_board_j
 * @param inner_i
 * @param inner_j
 * @return
 */
int check_value(sudoku_board_t *board, int value, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    return check_value_flattened(board, value, FULL_CONVERSION);
}

/**
 * Checks if all cells in the board are set
 * @param board
 * @return
 */
int is_board_full(const sudoku_board_t *board) {
    int i, j;
    for (i = 0; i < board->total_rows; ++i) {
        for (j = 0; j < board->total_cols; ++j) {
            if (get_cell_flattened(board, i, j) == 0) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/**
 * Counts number of empty cells in board
 * @param board
 * @return
 */
int empty_cells_num(const sudoku_board_t *board) {
    int i, j;
    int num = 0;

    for (i = 0; i < board->total_size; ++i) {
        for(j = 0; j < board->total_size; ++j) {
            if (get_cell_flattened(board, i, j) == 0) {
                num += 1;
            }
        }
    }
    return num;
}

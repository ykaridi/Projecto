#include "sudoku_game.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

#define EXIT_ON_ERROR(code) \
    fprintf(stderr, "Error: %s has failed\n", code); \
    exit(0);

/**
 * Dynamically allocates a sudoku board object
 * @param rows Number of rows
 * @param cols Number of columns
 * @return Sudoku board object
 */
sudoku_board_t *create_board(int rows, int cols) {
    int i;

    sudoku_board_t *board = malloc(sizeof(sudoku_board_t));
    if (board == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    board->rows = rows;
    board->cols = cols;
    board->total_rows = rows * rows;
    board->total_cols = cols * cols;
    board->sub_board_size = rows * cols;

    board->board = malloc(sizeof(int) * rows * rows * cols * cols);
    board->cell_metadata = malloc(sizeof(char) * rows * rows * cols * cols);
    if (board->board == NULL || board->cell_metadata == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    for (i = 0; i < rows * rows * cols * cols; i++) {
        board->board[i] = EMPTY_CELL;
        board->cell_metadata[i] = EMPTY_METADATA;
    }

    return board;
}

/**
 * Destructs (frees) a sudoku board object
 * @param board Sudoku board object
 */
void destruct_board(sudoku_board_t *board) {
    free(board->board);
    free(board->cell_metadata);
    free(board);
}

/**
 * Clears board of temporary cells
 * @param board Sudoku board object
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
 * Checks if an index is valid
 * @param board Sudoku board object
 * @param sub_board_i
 * @param sub_board_j
 * @param inner_i
 * @param inner_j
 * @return 1 = In board, 0 = Invalid (not in board)
 */
int is_in_board(const sudoku_board_t *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    return in_range(sub_board_i, 0, board->rows) && in_range(sub_board_j, 0, board->cols) &&
           in_range(inner_i, 0, board->rows) && in_range(inner_j, 0, board->cols);
}
/**
 * Checks if an index is valid
 * @param board Sudoku board object
 * @param i
 * @param j
 * @return 1 = In board, 0 = Invalid (not in board)
 */
int is_in_board_flattened(const sudoku_board_t *board, int i, int j) {
    return is_in_board(board, FLAT_CONVERSION);
}

/**
 * Returns a cell's value
 * @param board Sudoku board object
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
 * @param board Sudoku board object
 * @param i
 * @param j
 * @return Cell value (or ERROR on error)
 */
int get_cell_flattened(const sudoku_board_t *board, int i, int j) {
    return get_cell(board, FLAT_CONVERSION);
}
/**
 * Sets a cell's value
 * @param board Sudoku board object
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
    if (get_cell_metadata(board, sub_board_i, sub_board_j, inner_i, inner_j) == FIXED_METADATA)
        return ERROR;

    board->board[ARR_CONVERSION] = value;
    return SUCCESS;
}
/**
 * Sets a cell's value
 * @param board Sudoku board object
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
 * @param board Sudoku board object
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
 * @param board Sudoku board object
 * @param i
 * @param j
 * @return Cell metadata (or ERROR on error)
 */
char get_cell_metadata_flattened(const sudoku_board_t *board, int i, int j) {
    return get_cell_metadata(board, FLAT_CONVERSION);
}
/**
 * Sets a cell's metadata
 * @param board Sudoku board object
 * @param metadata Cell's new metadata
 * @param sub_board_i
 * @param sub_board_j
 * @param inner_i
 * @param inner_j
 * @return Success flag
 */
int set_cell_metadata(sudoku_board_t *board, char metadata, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    if (!is_in_board(board, sub_board_i, sub_board_j, inner_i, inner_j))
        return ERROR;
    if (get_cell_metadata(board, sub_board_i, sub_board_j, inner_i, inner_j) == FIXED_METADATA)
        return ERROR;

    board->cell_metadata[ARR_CONVERSION] = metadata;
    return 0;
}
/**
 * Sets a cell's metadata
 * @param board Sudoku board object
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
            if (current_cell == value)
                return FALSE;
        } else {
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
            if (current_cell == value)
                return FALSE;
        } else {
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
                if (current_cell == value)
                    return FALSE;
            } else {
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
 * Checks if a board is solved correctly
 * @param board Sudoku board object
 * @return success flag
 */
int check_board(const sudoku_board_t *board) {
    int i, j = 0;
    for (i = 0; i < board->total_rows; i++) {
        if (check_row(board, i, 0) == FALSE)
            return FALSE;
    }
    for (j = 0; j < board->total_cols; j++) {
        if (check_column(board, j, 0) == FALSE)
            return FALSE;
    }
    for (i = 0; i < board->rows; i++) {
        for(j = 0; j < board->cols; j++) {
            if (check_sub_board(board, i, j, 0) == FALSE)
                return FALSE;
        }
    }

    return TRUE;
}

#include "sudoku_game.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

#define FLAT_CONVERSION i / board->cols, j / board->rows, i % board->cols, j % board->rows
#define ARR_CONVERSION (sub_board_i * board->cols + sub_board_j) * board->sub_board_size +\
                        (inner_i * board->cols + inner_j)

#define EXIT_ON_ERROR(code) ({ \
    fprintf(stderr, "Error: %s has failed\n", code); \
    exit(0); \
})

/// Dynamically allocates a sudoku board object
/// \param rows Number of rows
/// \param cols Number of columns
/// \return Soduku board object
sudoku_board_t *create_board(int rows, int cols) {
    sudoku_board_t *board = malloc(sizeof(sudoku_board_t));
    if (board == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    board->rows = rows;
    board->cols = cols;
    board->sub_board_size = rows * cols;

    board->board = malloc(sizeof(int) * rows * rows * cols * cols);
    board->cell_metadata = malloc(sizeof(char) * rows * rows * cols * cols);
    if (board->board == NULL || board->cell_metadata == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    for (int i = 0; i < rows * rows * cols * cols; i++) {
        board->board[i] = 0;
        board->cell_metadata[i] = ' ';
    }

    return board;
}

/// Destructs (frees) a sudoku board object
/// \param board Sudoku board object
void destruct_board(sudoku_board_t *board) {
    free(board->board);
    free(board->cell_metadata);
    free(board);
}

/// Checks if an index is valid
/// \param board Sudoku board object
/// \param sub_board_i
/// \param sub_board_j
/// \param inner_i
/// \param inner_j
/// \return 1 = In board, 0 = Invalid (not in board)
int is_in_board(const sudoku_board_t *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    return in_range(sub_board_i, 0, board->rows) && in_range(sub_board_j, 0, board->cols) &&
           in_range(inner_i, 0, board->rows) && in_range(inner_j, 0, board->cols);
}
/// Checks if an index is valid
/// \param board Sudoku board object
/// \param i
/// \param j
/// \return 1 = In board, 0 = Invalid (not in board)
int is_in_board_flattened(const sudoku_board_t *board, int i, int j) {
    return is_in_board(board, FLAT_CONVERSION);
}

/// Returns a cell's value
/// \param board Sudoku board object
/// \param sub_board_i
/// \param sub_board_j
/// \param inner_i
/// \param inner_j
/// \return Cell value
int get_cell(const sudoku_board_t *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    if (!is_in_board(board, sub_board_i, sub_board_j, inner_i, inner_j))
        return ERROR;
    return board->board[ARR_CONVERSION];
}

int get_cell_flattened(const sudoku_board_t *board, int i, int j) {
    return get_cell(board, FLAT_CONVERSION);
}

int set_cell(sudoku_board_t *board, int value, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    board->board[ARR_CONVERSION] = value;
    return 0;
}

int set_cell_flattened(sudoku_board_t *board, int value, int i, int j) {
    return set_cell(board, value, FLAT_CONVERSION);
}

char get_cell_metadata(const sudoku_board_t *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    return board->cell_metadata[ARR_CONVERSION];
}

char get_cell_metadata_flattened(const sudoku_board_t *board, int i, int j) {
    return get_cell_metadata(board, FLAT_CONVERSION);
}

int set_cell_metadata(sudoku_board_t *board, char metadata, int sub_board_i, int sub_board_j, int inner_i, int inner_j) {
    board->cell_metadata[ARR_CONVERSION] = metadata;
    return 0;
}

int set_cell_metadata_flattened(sudoku_board_t *board, char metadata, int i, int j) {
    return set_cell_metadata(board, metadata, FLAT_CONVERSION);
}

int check_row(int row, int idx, int value);
int check_column(int row, int idx, int value);
int check_sub_board(int sub_board_i, int sub_board_j, int idx_i, int idx_j);
#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#define TRUE (1)
#define FALSE (0)
#define ERROR (-1)

struct _sudoku_board_t {
    int rows;
    int cols;
    int sub_board_size;

    int* board;
    char* cell_metadata;
} typedef sudoku_board_t;

sudoku_board_t* create_board(int rows, int cols);
void destruct_board(sudoku_board_t *board);

int is_in_board(const sudoku_board_t* board, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
int is_in_board_flattened(const sudoku_board_t *board, int i, int j);

int get_cell(const sudoku_board_t *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
int get_cell_flattened(const sudoku_board_t *board, int i, int j);
int set_cell(sudoku_board_t *board, int value, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
int set_cell_flattened(sudoku_board_t *board, int value, int i, int j);

/*
 * Cell metadata convention
 * ========================
 * ' ' - None
 * 'T' - Temporary (Backtracking)
 * '.' - Fixed
 */

char get_cell_metadata(const sudoku_board_t *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
char get_cell_metadata_flattened(const sudoku_board_t *board, int i, int j);
int set_cell_metadata(sudoku_board_t *board, char metadata, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
int set_cell_metadata_flattened(sudoku_board_t *board, char metadata, int i, int j);

int check_row(int row, int idx, int value);
int check_column(int row, int idx, int value);
int check_sub_board(int sub_board_i, int sub_board_j, int idx_i, int idx_j);

#endif
#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#define TRUE (1)
#define FALSE (0)
#define ERROR (-1)
#define SUCCESS (0)

#define SUB_BOARD_CONVERSION i / board->cols, j / board->rows
#define FLAT_CONVERSION i / board->cols, j / board->rows, i % board->cols, j % board->rows
#define ARR_CONVERSION (sub_board_i * board->cols + sub_board_j) * board->sub_board_size +\
                        (inner_i * board->cols + inner_j)

#define EMPTY_CELL (0)

#define FIXED_METADATA ('.')
#define TEMPORARY_METADATA ('T')
#define EMPTY_METADATA (' ')

#ifndef NULL
#define NULL (0)
#endif

typedef struct _sudoku_board_t {
    int rows;
    int cols;
    int total_rows;
    int total_cols;
    int sub_board_size;

    int* board;
    char* cell_metadata;
} sudoku_board_t;

sudoku_board_t* create_board(int rows, int cols);
void destruct_board(sudoku_board_t *board);
void clear_board_temps(sudoku_board_t *board);

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

int check_row(const sudoku_board_t *board, int row, int value);
int check_column(const sudoku_board_t *board, int col, int value);
int check_sub_board(const sudoku_board_t *board, int sub_board_i, int sub_board_j, int value);
int check_board(const sudoku_board_t *board);

#endif

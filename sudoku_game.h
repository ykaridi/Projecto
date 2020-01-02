#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H


struct _sudoku_board {
    int rows;
    int cols;
    int sub_board_size;

    int* board;
    char* cell_metadata;
} typedef sudoku_board;

sudoku_board* create_board(int rows, int cols);
void destruct_board(sudoku_board *board);

int is_in_board(const sudoku_board* board, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
int is_in_board_flattened(const sudoku_board *board, int i, int j);

int get_cell(const sudoku_board *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
int get_cell_flattened(const sudoku_board *board, int i, int j);
int set_cell(sudoku_board *board, int value, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
int set_cell_flattened(sudoku_board *board, int value, int i, int j);
char get_cell_metadata(const sudoku_board *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
char get_cell_metadata_flattened(const sudoku_board *board, int i, int j);
int set_cell_metadata(sudoku_board *board, char metadata, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
int set_cell_metadata_flattened(sudoku_board *board, char metadata, int i, int j);

#endif
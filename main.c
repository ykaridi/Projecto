#include <stdio.h>
#include "main_aux.h"

#define ROWS (5)
#define COLS (3)

int main() {
    sudoku_board board = *create_board(ROWS, COLS);
    for(int sub_board_i = 0; sub_board_i < board.rows; sub_board_i++) {
        for (int inner_i = 0; inner_i < board.rows; inner_i++) {
            for (int sub_board_j = 0; sub_board_j < board.cols; sub_board_j++) {
                for (int inner_j = 0; inner_j < board.cols; inner_j++) {
                    set_cell(&board, inner_i * ROWS + inner_j + 1, sub_board_i, sub_board_j, inner_i, inner_j);
                    set_cell_metadata(&board, '.', sub_board_i, sub_board_j, inner_i, inner_j);
                }
            }
        }
    }

    print_board(&board);
}
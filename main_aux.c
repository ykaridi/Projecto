#include "main_aux.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Returns length of number in some base
 * @param x Number
 * @param base Base
 * @return
 */
int num_length(int x, int base) {
    int result = 0;

    while (x > 0) {
        result++;
        x /= base;
    }

    return result;
}

/**
 * Prints a certain string for several times
 * @param s
 * @param times
 */
void print_repeated(char* s, int times) {
    int i;

    for (i = 0; i < times; i++) {
        printf("%s", s);
    }
}

/**
 * Prints a sudoku board
 * @param board
 */
void print_board(const sudoku_board_t *board) {
    int sub_board_i, inner_i, sub_board_j, inner_j, cell_value = 0;
    int n_len = num_length(board->sub_board_size, 10);
    int total_len = (2 + (n_len + 2) * board->cols) * board->cols + 1;

    for(sub_board_i = 0; sub_board_i < board->rows; sub_board_i++) {
        print_repeated("-", total_len);
        printf("\n");
        for (inner_i = 0; inner_i < board->rows; inner_i++) {
            for (sub_board_j = 0; sub_board_j < board->cols; sub_board_j++) {
                printf("| ");
                for (inner_j = 0; inner_j < board->cols; inner_j++) {
                    cell_value = get_cell(board, sub_board_i, sub_board_j, inner_i, inner_j);

                    /* TODO: Switch in final project */
                    printf("%c", get_cell_metadata(board, sub_board_i, sub_board_j, inner_i, inner_j));
                    if (cell_value == 0)
                        printf("%*s", n_len, "");
                    else {
                        int value_length = num_length(cell_value, 10);
                        printf("%*s%d", n_len - value_length, "", cell_value);
                    }

                    printf(" ");
                }
            }
            printf("|\n");
        }
    }
    print_repeated("-", total_len);
    printf("\n");
}

/**
 * Initialize the fixed cells.
 * @return number of fixed cells, or ERROR if there was an error.
 */
int get_num_fixed_cells(sudoku_game_t *game) {
    int num_fixed = -1;
    printf("Please enter the number of cells to fill [0-%d]:\n", game->total_rows * game->total_cols - 1);

    while (num_fixed < 0) {
        if (scanf("%d", &num_fixed) < 0) {
            if (IS_EOF)
                return ERROR;
            EXIT_ON_ERROR("scanf");
        }

        if (num_fixed < 0 || num_fixed > game->total_rows * game->total_cols - 1) {
            printf("\"Error: invalid number of cells to fill (should be between 0 and %d)\n", game->total_rows * game->total_cols - 1);
            num_fixed = -1;
        }
    }

    return num_fixed;
}

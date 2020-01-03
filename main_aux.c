#include "main_aux.h"
#include <stdio.h>

int num_length(int x, int base) {
    int result = 0;

    while (x > 0) {
        result++;
        x /= base;
    }

    return result;
}

void print_repeated(char* s, int times) {
    int i;

    for (i = 0; i < times; i++) {
        printf("%s", s);
    }
}

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
                    /* TODO: Note for project, reverse order of metadata & value */
                    printf("%c", get_cell_metadata(board, sub_board_i, sub_board_j, inner_i, inner_j));

                    cell_value = get_cell(board, sub_board_i, sub_board_j, inner_i, inner_j);
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

/** Initialize the fixed cells.
 *
 * @return number of fixed cells, or ERROR if there was an error.
 */
int get_fixed_cells() {
    printf("Please enter the number of cells to fill [0-%d]:\n", FIXED_CELLS_NUM);
    return 0;
}
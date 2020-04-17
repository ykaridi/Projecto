#include "sudoku_fs.h"

#define BAD_FILE    {   destruct_board(board); \
                        *board_ptr = NULL; \
                        return LOAD_EBADFILE; \
                    }

/**
 * Saves the given board to the given file.
 * @param pathname  file path
 * @param board     suduko board to save
 * @return SUCCESS/ERROR. errno is set on error.
 */
int save_board_to_file(const char* pathname, const sudoku_board_t* board) {
    int m = board -> rows, n = board -> cols;
    int i, j;

    FILE* file = fopen(pathname, "w");
    if (!file) {
        return ERROR;
    }

    fprintf(file, "%d %d\n", m, n);
    for(i = 0; i < board -> total_rows; ++i) {
        for(j = 0; j < board -> total_cols; ++j) {
            fprintf(file, "%d", get_cell_flattened(board, i, j));
            if (get_cell_metadata_flattened(board, i, j) == FIXED_METADATA) {
                fprintf(file, ".");
            }
            if (j < board -> total_cols - 1) {
                fprintf(file, " ");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);

    return SUCCESS;
}

/**
 * Loads a board from the given path.
 * @param pathname  file path
 * @param board     the address of the pointer to board.
 * @return the status of the function.
 */
enum load_file_status board_from_file(const char* pathname, sudoku_board_t** board_ptr) {
    int m, n, i = 0, j = 0;
    int cell;
    sudoku_board_t* board;

    FILE *file = fopen(pathname, "r");
    if (!file) {
        return LOAD_ENOFILE;
    }

    if (fscanf(file, "%d %d\n", &m, &n) < 2) {
        *board_ptr = NULL;
        return LOAD_EBADFILE;
    }
    if (m <= 0 || n <= 0) {
        *board_ptr = NULL;
        return LOAD_EBADFILE;
    }
    board = *board_ptr = create_board(m, n);

    for (i = 0; i < board->total_rows; ++i) {
        for(j = 0; j < board->total_cols; ++j) {
            int test_dot = 0;
            if (fscanf(file, " %d ", &cell) < 1) {
                BAD_FILE
            }
            if (cell < 0 || cell > board->total_rows) {
                BAD_FILE
            }
            set_cell_flattened(board, cell, i, j);
            fscanf(file, " . %n", &test_dot);
            if (test_dot > 0) {
                if (cell == 0) {
                    BAD_FILE
                }
                set_cell_metadata_flattened(board, FIXED_METADATA, i, j);
            }
        }
    }

    fclose(file);
    return LOAD_SUCCESS;
}

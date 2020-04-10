#include <stdlib.h>

#include "random_value_chooser.h"
#include "../../utils.h"

/**
 * Creates a random value chooser.
 * @param size
 * @return
 */
rand_value_chooser_t* create_chooser(int size) {
    rand_value_chooser_t* chooser = malloc(sizeof(rand_value_chooser_t));
    chooser->workspace = malloc(size * sizeof(int));
    if (!chooser->workspace) {
        EXIT_ON_ERROR("malloc");
    }
    return chooser;
}

/**
 * Destruct a random value chooser.
 * @param chooser
 */
void destruct_chooser(rand_value_chooser_t* chooser) {
    free(chooser->workspace);
    free(chooser);
}

/**
 * Choose a random legal value for cell.
 * @param board
 * @param cell
 * @param cleanup
 * @return
 */
int choose_random_legal_value(rand_value_chooser_t* chooser, sudoku_board_t *board, int row, int col) {
    int len = 0, i = row, j = col, k;

    for (k = 0; k < board->sub_board_size; ++k) {
        if (check_value_flattened(board, k + 1, i, j)) {
            chooser->workspace[len++] = k + 1;
        }
    }
    if (len == 0) {
        return 0;
    }

    if (len == 1) {
        return chooser->workspace[0];
    } else {
        return chooser->workspace[rand() % len];
    }
}

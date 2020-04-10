#ifndef RANDOM_VALUE_CHOOSER_H
#define RANDOM_VALUE_CHOOSER_H

#include "../sudoku_board.h"

/**
 * This struct is used by choose_random_legal_value.
 * Before using that function, you must create this struct (using the create chooser function).
 * After done with the function, you can destruct it using the destructor.
 */
typedef struct _rand_value_chooser {
    int *workspace;
} rand_value_chooser_t;

rand_value_chooser_t* create_chooser(int size);
void destruct_chooser(rand_value_chooser_t* chooser);

int choose_random_legal_value(rand_value_chooser_t* chooser, sudoku_board_t *board, int row, int col);

#endif /* RANDOM_VALUE_CHOOSER_H */

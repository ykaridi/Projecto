#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "sudoku_solver.h"

/* IMPORTANT NOTICE: WHEN CHANGING THESE VALUES SHOULD CHANGE ALSO NUM_VALUES IN SUDOKU_SOLVER */
#define ROWS (3)
#define COLS (3)

int main(int argc, char* argv[]) {
    sudoku_board_t board;

    if (argc < 2) {
        printf("Syntax: %s <seed>", argv[0]);
        exit(1);
    }
    srand((unsigned) atoi(argv[1]));

    board = create_board(ROWS, COLS);
    randomize_board(&board, 8);

    print_board(&board);
    if (check_board(&board) == TRUE) {
        printf("Yay! Valid board!\n");
    } else {
        printf("Boo! Invalid board!\n");
    }

    destruct_board(&board);
    return 0;
}

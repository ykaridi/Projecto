#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "sudoku_solver.h"

#define ROWS (3)
#define COLS (3)

int main(int argc, char* argv[]) {
    srand((unsigned) atoi(argv[1]));

    sudoku_board_t *board = create_board(ROWS, COLS);

    randomize_board(board, 8);
    //deterministic_backtracking(board, 0, 0);

    print_board(board);
    if (check_board(board) == TRUE) {
        printf("Yay! Valid board!\n");
    } else {
        printf("Boo! Invalid board!\n");
    }

    destruct_board(board);
}
#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "sudoku_solver.h"

/* IMPORTANT NOTICE: WHEN CHANGING THESE VALUES SHOULD CHANGE ALSO NUM_VALUES IN SUDOKU_SOLVER */
#define ROWS (3)
#define COLS (3)

int main(int argc, char* argv[]) {
    sudoku_game_t game;

    if (argc < 2) {
        printf("Syntax: %s <seed>", argv[0]);
        exit(1);
    }
    srand((unsigned) atoi(argv[1]));

    game = create_game(ROWS, COLS);
    update_solution(&game, FALSE);
    reveal_cells(&game, 8);

    print_board(game.board);
    if (check_board(game.board) == TRUE) {
        printf("Yay! Valid board!\n");
    } else {
        printf("Boo! Invalid board!\n");
    }

    destruct_board(game.board);
    return 0;
}

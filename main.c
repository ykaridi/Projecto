#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "sudoku_solver.h"
#include "utils.h"

/* IMPORTANT NOTICE: WHEN CHANGING THESE VALUES SHOULD CHANGE ALSO NUM_VALUES IN SUDOKU_SOLVER */
#define ROWS (3)
#define COLS (3)

void exit_gracefully(sudoku_game_t *game) {
    destruct_game(game);
    printf("Exiting...\n");
    exit(0);
}

int main(int argc, char* argv[]) {
    sudoku_game_t game;
    int num_fixed_cells;

    if (argc < 2) {
        printf("Syntax: %s <seed>", argv[0]);
        exit(1);
    }
    srand((unsigned) atoi(argv[1]));

    game = create_game(ROWS, COLS);
    num_fixed_cells = get_num_fixed_cells(&game);
    if (num_fixed_cells < 0)
        exit_gracefully(&game);

    update_solution(&game, FALSE);
    reveal_cells(&game, num_fixed_cells);

    print_board(game.board);
    if (check_board(game.board) == TRUE) {
        printf("Yay! Valid board!\n");
    } else {
        printf("Boo! Invalid board!\n");
    }

    destruct_board(game.board);
    return 0;
}
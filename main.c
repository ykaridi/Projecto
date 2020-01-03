#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "sudoku_solver.h"
#include "utils.h"
#include "parser.h"

/* IMPORTANT NOTICE: WHEN CHANGING THESE VALUES SHOULD CHANGE ALSO NUM_VALUES IN SUDOKU_SOLVER */
#define ROWS (3)
#define COLS (3)

#define MAX_COMMAND_LINE_LEN 1024

void exit_gracefully(sudoku_game_t *game) {
    destruct_game(game);
    printf("Exiting...\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    sudoku_game_t game;
    int num_fixed_cells;
    char command_line[MAX_COMMAND_LINE_LEN];
    command_t command;
    command_args_t command_arguments;
    command_output_t command_output = {DONE};

    if (argc < 2) {
        printf("Syntax: %s <seed>", argv[0]);
        exit(1);
    }
    srand((unsigned) atoi(argv[1]));

    while (command_output.exit_code != EXIT_PROGRAM) {
        game = create_game(ROWS, COLS);
        num_fixed_cells = get_num_fixed_cells(&game);
        if (num_fixed_cells < 0)
            exit_gracefully(&game);
        update_solution(&game, FALSE);
        reveal_cells(&game, num_fixed_cells);
        print_board(game.board);

        getchar();
        do {
            do {
                fgets(command_line, MAX_COMMAND_LINE_LEN, stdin);
            } while (parse_command(command_line, &command, &command_arguments) != SUCCESS);
            command_output = command.function(&game, command_arguments);
        } while (command_output.exit_code == DONE);
    }

    exit_gracefully(&game);
    return 0;
}
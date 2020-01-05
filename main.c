#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "sudoku_solver.h"
#include "utils.h"
#include "parser.h"
#include "sudoku_commands.h"

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
    command_list_t commands;
    command_t _commands[] = {
            {
                    set, "set"
            },
            {
                    hint, "hint"
            },
            {
                    validate, "validate"
            },
            {
                    restart, "restart"
            },
            {
                    exit_sudoku, "exit"
            }
    };

    commands.num_commands = sizeof(_commands) / sizeof(command_t);
    commands.commands = _commands;

    if (argc < 2) {
        printf("Syntax: %s <seed>\n", argv[0]);
        exit(1);
    }
    srand((unsigned) atoi(argv[1]));

    while (command_output.exit_code != EXIT_PROGRAM) {
        game = create_game(ROWS, COLS);
        num_fixed_cells = get_num_fixed_cells(&game);
        if (num_fixed_cells < 0 || num_fixed_cells > 80)
            exit_gracefully(&game);;
        update_solution(&game, FALSE);
        reveal_cells(&game, num_fixed_cells);
        print_board(game.board);

        if (IS_EOF)
            exit_gracefully(&game);
        getchar();
        do {
            do {
                if (IS_EOF)
                    exit_gracefully(&game);
                fgets(command_line, MAX_COMMAND_LINE_LEN, stdin);
                if (IS_EOF)
                    exit_gracefully(&game);
            } while (parse_command(command_line, &commands, &command, &command_arguments) != SUCCESS);
            command_output = command.function(&game, command_arguments);
        } while (command_output.exit_code == DONE);

        destruct_game(&game);
    }

    printf("Exiting...\n");
    return 0;
}

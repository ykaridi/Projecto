#include <stdio.h>
#include <stdlib.h>
#include "engine/commands/sudoku_commands.h"
#include "engine/commands/parser.h"
#include "engine/user_interface.h"

#define MAX_COMMAND_LINE_LEN (256)

void exit_gracefully(sudoku_game_t *game) {
    destruct_game(game);
    printf("Exiting...\n");
    exit(0);
}

int main() {
    sudoku_game_t *game = create_game();
    command_list_t command_list;
    char command_line[MAX_COMMAND_LINE_LEN * 2];

    const command_t *command;
    command_arguments_t command_arguments;
    enum command_status command_status = DONE;

    int parsing_status;
    parsing_errors_t parsing_error;

    load_commands(&command_list);

    printf("/=========\\\n");
    printf("| Sudoku! | \n");
    printf("\\=========/\n");
    printf("\n");

    do {
        if (command_status == BOARD_UPDATE || command_status == PARAMETER_UPDATE) {
             update_arguments(&command_list, game);
        }

        do {
            if (IS_EOF)
                exit_gracefully(game);
            printf(">>> ");
            do {
                fgets(command_line, MAX_COMMAND_LINE_LEN, stdin);
                if (IS_EOF)
                    exit_gracefully(game);
                parsing_status = parse_command(command_line, game->mode, &command_list, &command, &command_arguments,
                                               &parsing_error);
                if (parsing_status && parsing_error.error_type != EMPTY_COMMAND) {
                    print_parsing_error(&parsing_error, command);
                }
            } while (parsing_error.error_type == EMPTY_COMMAND);
        } while (parsing_status);
        command_status = (*command).function(game, &command_arguments);
        if (command_status == BOARD_UPDATE || command_status == PARAMETER_UPDATE) {
            print_board(game->board, game->mode == EDIT || game->mark_errors, game->mode != EDIT);
            if (game->mode == SOLVE && is_board_full(game->board)) {
                if (check_board(game->board)) {
                    printf("Board was solved successfully!\n");
                    game->mode = INIT;
                } else {
                    printf("Board is erroneous!\n");
                }
            }
        }
    } while (command_status != EXIT_PROGRAM);

    exit_gracefully(game);
    return 1;
}

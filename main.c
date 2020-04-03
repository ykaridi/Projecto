#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "engine/fs/sudoku_fs.h"
#include "engine/commands/sudoku_commands.h"

#define MAX_COMMAND_LINE_LEN (256)

void exit_gracefully(sudoku_game_t *game) {
    /* destruct_game(game); */
    printf("Exiting...\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    sudoku_game_t game;
    command_list_t command_list;
    char command_line[MAX_COMMAND_LINE_LEN * 2];

    const command_t *command;
    command_arguments_t command_arguments;
    enum command_status command_status = UPDATE_COMMANDS;

    int parsing_status = FALSE;
    parsing_errors_t parsing_error;

    load_commands(&command_list);
    game.mode = SOLVE;

    do {
        if (command_status == UPDATE_COMMANDS)
            update_arguments(&command_list, &game);

        do {
            if (IS_EOF)
                exit_gracefully(&game);
            fgets(command_line, MAX_COMMAND_LINE_LEN, stdin);
            if (IS_EOF)
                exit_gracefully(&game);
            parsing_status = parse_command(command_line, game.mode, &command_list, &command, &command_arguments,
                                           &parsing_error);
            if (parsing_status) {
                print_parsing_error(&parsing_error, command);
            }
        } while (parsing_status);
        command_status = (*command).function(&game, &command_arguments);
        printf("Executed command [%s] successfully!\n", command->command_name);
    } while (command_status != EXIT_PROGRAM);

    exit_gracefully(&game);
}

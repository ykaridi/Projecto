#include <stdio.h>
#include <stdlib.h>
#include "main_aux.h"
#include "utils.h"
#include "engine/commands/parser.h"
#include "engine/commands/commands.h"
#include "engine/sudoku_game.h"

#define COMMAND_LEN (256)

int main(int argc, char *argv[]) {
    command_argument_description_t integer_arg = {
            .argument_type = INTEGER,
            .enforce_integer_range = TRUE,
            .lower_bound = 0,
            .upper_bound = 10
    };
    command_t command = {
            .command_name = "test",
            .modes_of_availability = {
                    .init = 1,
                    .edit = 1,
                    .solve = 1
            },
            .args = {
                    .num_arguments = 1,
                    .arguments = {
                            &integer_arg, &integer_arg, &integer_arg
                    }
            },
            .function = NULL
    };
    command_t* _commands[] = { &command };
    command_t *input_command;
    command_arguments_t args;
    command_list_t commands = {
            .num_commands = 1,
            .commands = _commands
    };
    parsing_errors_t parsing_errors;

    char command_text[256];
    fgets(command_text, COMMAND_LEN, stdin);
    if (parse_command(command_text, 0, &commands, &input_command, &args, &parsing_errors) != 0) {
        print_parsing_error(&parsing_errors, input_command);
    }

    printf("Yay!\n");
}

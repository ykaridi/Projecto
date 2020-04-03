#include "parser.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int parse_command(const char *text, enum sudoku_mode mode, const command_list_t *commands, command_t **command_ptr,
                  command_arguments_t *args, parsing_errors_t *errors) {
    char *token = NULL, *command_name = NULL, *strtol_end = NULL, strtol_char[2] = "0\x00";
    int i = 0, num_arg = 0, value;
    long _value;
    command_t *command = NULL;

    command_name = strtok(text, DELIM);
    if (command_name == NULL) {
        errors->error_type = NO_COMMAND;
        return ERROR;
    }
    for (i = 0; i < commands->num_commands; ++i) {
        if (strcmp(commands->commands[i]->command_name, command_name) == 0) {
            command = commands->commands[i];
            *command_ptr = command;
            args->num_arguments = command->args.num_arguments;
        }
    }

    if (command == NULL) {
        errors->error_type = NO_COMMAND;
        return ERROR;
    } else if ((mode == INIT && !command->modes_of_availability.init) ||
    (mode == EDIT && !command->modes_of_availability.edit) ||
    (mode == SOLVE && !command->modes_of_availability.solve)) {
        errors->error_type = UNAVAILABLE_COMMAND;
        return ERROR;
    }

    token = strtok(NULL, DELIM);
    while ((token != NULL) && (num_arg < command->args.num_arguments)) {
        if (command->args.arguments[num_arg]->argument_type == INTEGER) {
            args->arguments[num_arg].type = INTEGER;

            _value = strtol(token, &strtol_end, 10);
            *strtol_char = *strtol_end;
            if (!strstr(DELIM_WITH_NULL, strtol_char)) {
                errors->error_type = INCORRECT_TYPE;
                errors->param_index = num_arg;
                return ERROR;
            }

            value = _value;

            if (command->args.arguments[num_arg]->enforce_integer_range)
                if (!in_range(value, command->args.arguments[num_arg]->lower_bound,
                        command->args.arguments[num_arg]->upper_bound)) {
                    errors->error_type = INCORRECT_RANGE;
                    errors->param_index = num_arg;
                    return ERROR;
                }

            args->arguments[num_arg].value.int_value = value;
        } else {
            args->arguments[num_arg].type = STRING;
            args->arguments[num_arg].value.str_value = token;
        }

        num_arg++;
        token = strtok(NULL, DELIM);
    }
    if (token != NULL) {
        errors->error_type = INCORRECT_NUM_PARAMS;
        return ERROR;
    }

    return SUCCESS;
}

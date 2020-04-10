#include "parser.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Parse a string to a it's appropriate command and arguments
 * @param text
 * @param mode Current game mode
 * @param commands List of all possible commands
 * @param command_ptr Pointer to write the pointer to the command
 * @param args Pointer to writing command arguments to
 * @param errors Pointer to write parsing errors into
 * @return
 */
int parse_command(char *text, enum sudoku_mode mode, const command_list_t *commands, command_t const **command_ptr,
                  command_arguments_t *args, parsing_errors_t *errors) {
    char *token = NULL, *command_name = NULL, *strtol_end = NULL, strtol_char[2] = "0\x00";
    int i = 0, num_arg = 0;
    const command_t *command = NULL;

    errors->error_type = NO_ERROR;

    /* Scan for command name */
    command_name = strtok(text, DELIM);
    if (command_name == NULL) {
        errors->error_type = EMPTY_COMMAND;
        return ERROR;
    }
    /* Search for a command with a matching name */
    for (i = 0; i < commands->num_commands; ++i) {
        if (strcmp(commands->commands[i].command_name, command_name) == 0) {
            command = &commands->commands[i];
            *command_ptr = command;
        }
    }

    /* Check if a matching command was found, and if so check if it is currently available */
    if (command == NULL) {
        errors->error_type = NO_COMMAND;
        return ERROR;
    } else if ((mode == INIT && !command->modes_of_availability.init) ||
    (mode == EDIT && !command->modes_of_availability.edit) ||
    (mode == SOLVE && !command->modes_of_availability.solve)) {
        errors->error_type = UNAVAILABLE_COMMAND;
        return ERROR;
    }

    /* Parse arguments */
    token = strtok(NULL, DELIM);
    while ((token != NULL) && (num_arg < command->args.num_arguments)) {
        /* Temporarily null terminate current argument */
        token[-1] = 0;
        /* Check expected argument type */
        if (command->args.arguments[num_arg]->argument_type == INTEGER ||
                command->args.arguments[num_arg]->argument_type == FLOAT) {
            args->arguments[num_arg].type = command->args.arguments[num_arg]->argument_type;

            /* Convert string argument to integer / float */
            if (args->arguments[num_arg].type == INTEGER) {
                args->arguments[num_arg].value.int_value = (int) strtol(token, &strtol_end, 10);
            } else {
                args->arguments[num_arg].value.float_value = (float) strtod(token, &strtol_end);
            }

            /* Check if conversion was successful */
            *strtol_char = *strtol_end;
            if (!strstr(DELIM_WITH_NULL, strtol_char) && errors->error_type != NO_ERROR) {
                errors->error_type = INCORRECT_TYPE;
                errors->param_index = num_arg;
            }

            /* Check if argument fits the expected range */
            if (errors->error_type == NO_ERROR) {
                if (args->arguments[num_arg].type == INTEGER && command->args.arguments[num_arg]->enforce_range) {
                    if (!in_range(args->arguments[num_arg].value.int_value,
                                  command->args.arguments[num_arg]->lower_bound.int_value,
                                  command->args.arguments[num_arg]->upper_bound.int_value + 1)) {
                        errors->error_type = INCORRECT_RANGE;
                        errors->param_index = num_arg;
                    }
                } else if (args->arguments[num_arg].type == FLOAT && command->args.arguments[num_arg]->enforce_range) {
                    if (!float_in_range(args->arguments[num_arg].value.float_value,
                                        command->args.arguments[num_arg]->lower_bound.float_value,
                                        command->args.arguments[num_arg]->upper_bound.float_value)) {
                        errors->error_type = INCORRECT_RANGE;
                        errors->param_index = num_arg;
                    }
                }
            }
        } else {
            args->arguments[num_arg].type = STRING;
            args->arguments[num_arg].value.str_value = token;
        }

        num_arg++;
        token = strtok(NULL, DELIM);
    }

    /* Check if there were additional arguments given by the user */
    if (token != NULL) {
        errors->error_type = INCORRECT_NUM_PARAMS;
        return ERROR;
    }
    args->num_arguments = num_arg;
    /* Check if enough arguments were given by user */
    if (num_arg < command->args.num_arguments) {
        /* Check if remaining arguments are optional */
        if  (command->args.arguments[num_arg]->optional) {
            for (; num_arg < command->args.num_arguments; num_arg++) {
                args->arguments[num_arg].supplied = FALSE;
            }
        } else {
            errors->error_type = INCORRECT_NUM_PARAMS;
            return ERROR;
        }
    }

    return errors->error_type == NO_ERROR ? SUCCESS : ERROR;
}

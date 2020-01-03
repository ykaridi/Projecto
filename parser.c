#include "parser.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/** parse the command given in text.
 * @param text      the command given in text form
 * @param command   the command
 * @param args      the arguments to the command
 * @return          did we manage to find a command
 */
int parse_command(char *text, command_list_t *commands, command_t *command, command_args_t *args) {
    char *token, *command_name;
    int i = 0, num_args = 0;

    command_name = strtok(text, DELIM);

    token = strtok(NULL, DELIM);
    while ((token != NULL) && (num_args < MAX_ARGS)) {
        (args->args)[num_args] = atoi(token);
        num_args++;
        token = strtok(NULL, DELIM);
    }
    args->num_args = num_args;

    for (i = 0; i < commands->num_commands; ++i) {
        if (strcmp(commands->commands[i].command_name, command_name) == 0) {
            *command = commands->commands[i];
            return SUCCESS;
        }
    }
    printf("Error: invalid command\n");
    return ERROR;
}

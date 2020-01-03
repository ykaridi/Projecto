#include <stdio.h>
#include "parser.h"
#include "main_aux.h"

#define ROWS (5)
#define COLS (3)

#include <string.h>

int main() {
    command_t command;
    command_args_t args;

    char text[] = "ex";

    parse_command(text, &command, &args);
    printf("command: %s\n", command.command_name);
    printf("args: %d %d %d %d\n", args.num_args, args.args[0], args.args[1], args.args[2]);
}
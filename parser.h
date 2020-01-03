#ifndef PARSER_H
#define PARSER_H

#include "sudoku_commands.h"

#define COMMAND_LEN (1024)

#define DELIM " \t\r\n"

int parse_command(char *text, command_list_t *commands, command_t *command, command_args_t *args);

#endif

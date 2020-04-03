#include "main_aux.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Returns length of number in some base
 * @param x Number
 * @param base Base
 * @return
 */
int num_length(int x, int base) {
    int result = 0;

    while (x > 0) {
        result++;
        x /= base;
    }

    return result;
}

/**
 * Prints a certain string for several times
 * @param s
 * @param times
 */
void print_repeated(char* s, int times) {
    int i;

    for (i = 0; i < times; i++) {
        printf("%s", s);
    }
}

/**
 * Prints a sudoku board
 * @param board
 */
void print_board(const sudoku_board_t *board) {
    int sub_board_i, inner_i, sub_board_j, inner_j, cell_value = 0;
    //int n_len = num_length(board->sub_board_size, 10);
    int n_len = 2;
    int total_len = (2 + (n_len + 2) * board->cols) * board->rows + 1;

    for(sub_board_i = 0; sub_board_i < board->cols; sub_board_i++) {
        print_repeated("-", total_len);
        printf("\n");
        for (inner_i = 0; inner_i < board->rows; inner_i++) {
            for (sub_board_j = 0; sub_board_j < board->rows; sub_board_j++) {
                printf("| ");
                for (inner_j = 0; inner_j < board->cols; inner_j++) {
                    cell_value = get_cell(board, sub_board_i, sub_board_j, inner_i, inner_j);

                    if (cell_value == 0)
                        printf("%*s", n_len, "");
                    else {
                        int value_length = num_length(cell_value, 10);
                        printf("%*s%d", n_len - value_length, "", cell_value);
                    }
                    printf("%c", get_cell_metadata(board, sub_board_i, sub_board_j, inner_i, inner_j));

                    printf(" ");
                }
            }
            printf("|\n");
        }
    }
    print_repeated("-", total_len);
    printf("\n");
}

void print_parsing_error(const parsing_errors_t *error, const command_t *command) {
    switch(error->error_type) {
        case NO_COMMAND:
            printf("Invalid command!\n");
            break;
        case UNAVAILABLE_COMMAND:
            printf("Command [%s] is currently unavailable! ", command->command_name);
            printf("Command is available in modes [");
            if (command->modes_of_availability.init)
                printf(" init ");
            if (command->modes_of_availability.edit)
                printf(" edit ");
            if (command->modes_of_availability.solve)
                printf(" solve ");
            printf("]\n");
            break;
        case INCORRECT_NUM_PARAMS:
            printf("Command [%s] takes %d parameters!\n", command->command_name, command->args.num_arguments);
            break;
        case INCORRECT_TYPE:
            printf("Command [%s] expects parameter [#%d] to be of type [",
                    command->command_name, error->param_index + 1);
            if (command->args.arguments[error->param_index]->argument_type == INTEGER)
                printf("INTEGER");
            else if (command->args.arguments[error->param_index]->argument_type == STRING)
                printf("STRING");
            else
                printf("OTHER");
            printf("]\n");
            break;
        case INCORRECT_RANGE:
            printf("Command [%s] expects parameter [#%d] to be an [INTEGER] in the range [%d, %d]!\n",
                    command->command_name, error->param_index + 1,
                    command->args.arguments[error->param_index]->lower_bound,
                    command->args.arguments[error->param_index]->upper_bound);
            break;
        default:
            printf("An unexpected error occurred!\n");
            break;
    }
}

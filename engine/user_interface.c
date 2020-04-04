#include "user_interface.h"

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
            printf("Command [%s] takes %d parameter(s)!\n", command->command_name, command->args.num_arguments);
            break;
        case INCORRECT_TYPE:
            printf("Command [%s] expects parameter [#%d] to be of type [",
                   command->command_name, error->param_index + 1);
            if (command->args.arguments[error->param_index]->argument_type == INTEGER)
                printf("INTEGER");
            else if (command->args.arguments[error->param_index]->argument_type == FLOAT)
                printf("FLOAT");
            else if (command->args.arguments[error->param_index]->argument_type == STRING)
                printf("STRING");
            else
                printf("OTHER");
            printf("]\n");
            break;
        case INCORRECT_RANGE:
            if (command->args.arguments[error->param_index]->argument_type == INTEGER)
                printf("Command [%s] expects parameter [#%d] to be a [INTEGER] in the range [%d, %d]!\n",
                       command->command_name, error->param_index + 1,
                       command->args.arguments[error->param_index]->lower_bound.int_value,
                       command->args.arguments[error->param_index]->upper_bound.int_value);
            else if (command->args.arguments[error->param_index]->argument_type == FLOAT)
                printf("Command [%s] expects parameter [#%d] to be a [FLOAT] in the range [%f, %f]!\n",
                       command->command_name, error->param_index + 1,
                       command->args.arguments[error->param_index]->lower_bound.float_value,
                       command->args.arguments[error->param_index]->upper_bound.float_value);
            break;
        default:
            printf("An unexpected error occurred!\n");
            break;
    }
}

void print_game_operation(const sudoku_game_operation_t *operation, int undo) {
    operation_node_t *node;
    if (operation->operation_type == ATOMIC) {
        printf("\tSet cell <%d, %d> to ", operation->value.atomic_operation.row + 1,
               operation->value.atomic_operation.col + 1);

        if (undo)
            printf("[%d]\n", operation->value.atomic_operation.old_value);
        else
            printf("[%d]\n", operation->value.atomic_operation.new_value);

        return;
    }

    if (undo)
        printf("Undo ");
    else
        printf("Redo ");

    switch (operation->operation_type) {
        case SET:
            printf("command [set %d %d %d]:\n", operation->value.composite_operation.arg1,
                    operation->value.composite_operation.arg2, operation->value.composite_operation.arg3);
            break;
        case AUTOFILL:
            printf("command [autofill]:\n");
            break;
        case GENERATE:
            printf("command [generate %d %d]:\n",
                    operation->value.composite_operation.arg1, operation->value.composite_operation.arg2);
            break;
        case GUESS:
            printf("command [guess %f]:\n", operation->value.composite_operation.farg);
            break;
        default:
            break;
    }

    node = operation->value.composite_operation.head;
    while (node != NULL) {
        print_game_operation(node->operation, undo);
        node = node->next;
    }
}

/**
 * Prints a sudoku board
 * @param board
 */
void print_board(const sudoku_board_t *board) {
    int sub_board_i, inner_i, sub_board_j, inner_j, cell_value = 0;
    /* int n_len = num_length(board->sub_board_size, 10); */
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

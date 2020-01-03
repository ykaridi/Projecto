#include "sudoku_commands.h"
#include "main_aux.h"

/**
 *  In this documentation, args.args = {x, y, z}
 */


/** Sets the value Z in cell <X,Y>
 * @param board
 * @param args
 * @return
 */
command_output_t set(sudoku_board_t *board, command_args_t args) {
    int j, i, z;
    if (args.num_args < 3) {
        printf("Error: invalid command\n");
        return (command_output_t) {SUCCESS};
    }

    j = args.args[0] - 1;
    i = args.args[0] - 1;
    z = args.args[0];

    if (get_cell_metadata_flattened(board, i, j) == FIXED_METADATA) {
        printf("Error: cell is fixed\n");
        return (command_output_t) {SUCCESS};
    }

    if (z == 0) {
        /* Clear cell */
        set_cell_flattened(board, z, i, j);
    } else {
        if (!check_row(board, i, z) || !check_column(board, j, z) || !check_sub_board(board, SUB_BOARD_CONVERSION, z)) {
            printf("Error: value is invalid\n");
            return (command_output_t) {SUCCESS};
        }
        set_cell_flattened(board, z, i, j);
    }

    print_board(board);
    return (command_output_t) {SUCCESS};
}

/*TODO*/
command_output_t hint(sudoku_board_t *board, command_args_t args) {
    return (command_output_t) {SUCCESS};
}

/*TODO*/
command_output_t validate(sudoku_board_t *board, command_args_t args) {
    return (command_output_t) {SUCCESS};
}

/*TODO*/
command_output_t restart(sudoku_board_t *board, command_args_t args) {
    return (command_output_t) {SUCCESS};
}

/*TODO*/
command_output_t exit_sudoku(sudoku_board_t *board, command_args_t args) {
    return (command_output_t) {SUCCESS};
}


command_t commands[] = {
        {
                .function =     set,
                .command_name = "set"
        },
        {
                .function =     hint,
                .command_name = "hint"
        },
        {
                .function =     validate,
                .command_name = "validate"
        },
        {
                .function =     restart,
                .command_name = "restart"
        },
        {
                .function =     exit_sudoku,
                .command_name = "exit"
        }
};
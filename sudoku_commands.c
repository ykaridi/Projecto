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
    if (args.num_args < 3 || is_board_full(board)) {
        printf("Error: invalid command\n");
        return (command_output_t) {DONE};
    }

    j = args.args[0] - 1;
    i = args.args[0] - 1;
    z = args.args[0];

    if (get_cell_metadata_flattened(board, i, j) == FIXED_METADATA) {
        printf("Error: cell is fixed\n");
        return (command_output_t) {DONE};
    }

    if (z == 0) {
        /* Clear cell */
        set_cell_flattened(board, z, i, j);
    } else {
        if (!check_value(board, z, i, j)) {
            printf("Error: value is invalid\n");
            return (command_output_t) {DONE};
        }
        set_cell_flattened(board, z, i, j);
    }

    print_board(board);

    if (is_board_full(board)) {
        printf("Puzzle solved successfully\n");
    }

    return (command_output_t) {DONE};
}

/*TODO*/
command_output_t hint(sudoku_board_t *board, command_args_t args) {
    if (args.num_args < 2 || is_board_full(board)) {
        printf("Error: invalid command\n");
        return (command_output_t) {DONE};
    }


    return (command_output_t) {DONE};
}

/*TODO*/
command_output_t validate(sudoku_board_t *board, command_args_t args) {
    if (is_board_full(board)) {
        printf("Error: invalid command\n");
        return (command_output_t) {DONE};
    }

    return (command_output_t) {DONE};
}

/** restart the game.
 * @param board
 * @param args
 * @return
 */
command_output_t restart(sudoku_board_t *board, command_args_t args) {
    return (command_output_t) {RESTART_GAME};
}

/** exit the program.
 * @param board
 * @param args
 * @return
 */
command_output_t exit_sudoku(sudoku_board_t *board, command_args_t args) {
    /* Just send the exit program signal */
    return (command_output_t) {EXIT_PROGRAM};
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
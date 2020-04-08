#include <errno.h>
#include <string.h>
#include "sudoku_commands.h"
#include "../fs/sudoku_fs.h"
#include "../solver/backtracking/sudoku_backtracking.h"
#include "../user_interface.h"
#include "../solver/gurobi/sudoku_grb.h"
#include "../random.h"

#define DEFAULT_NUM_ROWS (3)
#define DEFAULT_NUM_COlS (3)

enum command_status command_solve(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_edit(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_mark_errors(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_print_board(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_set(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_validate(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_guess(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_generate(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_undo(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_redo(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_save(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_hint(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_guess_hint(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_num_solutions(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_autofill(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_reset(sudoku_game_t *game, const command_arguments_t *args);

enum command_status command_exit(sudoku_game_t *game, const command_arguments_t *args);

command_t construct_command(const char *name, int num_args, command_argument_description_t *arg1,
                            command_argument_description_t *arg2, command_argument_description_t *arg3,
                            int available_init,
                            int available_edit, int available_solve,
                            enum command_status (*function)(sudoku_game_t *, const command_arguments_t *)) {
    command_t command;
    command_arguments_description_t *arguments = &command.args;
    modes_availability_t *modes_of_availability = &command.modes_of_availability;

    command.command_name = name;
    command.function = function;

    (*arguments).num_arguments = num_args;
    (*arguments).arguments[0] = arg1;
    (*arguments).arguments[1] = arg2;
    (*arguments).arguments[2] = arg3;

    (*modes_of_availability).init = available_init;
    (*modes_of_availability).edit = available_edit;
    (*modes_of_availability).solve = available_solve;

    return command;
}

void load_commands(command_list_t *command_list) {
    command_argument_description_t _generic_integer = {
            INTEGER, FALSE, FALSE, {-1}, {-1}
    };
    command_argument_description_t _generic_string = {
            STRING, FALSE, FALSE, {-1}, {-1}
    };
    command_argument_description_t _zo_float = {
            FLOAT, FALSE, FALSE, {0}, {1}
    };
    command_argument_description_t _boolean = {
            INTEGER, FALSE, TRUE, {0}, {1}
    };
    command_argument_description_t _integer_boardidx_min0 = {
            INTEGER, FALSE, TRUE, {0}, {-1}
    };
    command_argument_description_t _integer_boardidx_min1 = {
            INTEGER, FALSE, TRUE, {1}, {-1}
    };
    command_argument_description_t _integer_boardsize_min0 = {
            INTEGER, FALSE, TRUE, {1}, {-1}
    };
    command_argument_description_t _integer_boardsize_min1 = {
            INTEGER, FALSE, TRUE, {1}, {-1}
    };
    command_argument_description_t _optional_string = {
            STRING, TRUE, FALSE, {-1}, {-1}
    };

    command_argument_description_t *generic_integer = &command_list->possible_arguments.generic_integer;
    command_argument_description_t *generic_string = &command_list->possible_arguments.generic_string;
    command_argument_description_t *zo_float = &command_list->possible_arguments.zo_float;
    command_argument_description_t *boolean = &command_list->possible_arguments.boolean;
    command_argument_description_t *integer_boardidx_min0 = &command_list->possible_arguments.integer_boardidx_min0;
    command_argument_description_t *integer_boardidx_min1 = &command_list->possible_arguments.integer_boardidx_min1;
    command_argument_description_t *integer_boardsize_min0 = &command_list->possible_arguments.integer_boardsize_min0;
    command_argument_description_t *integer_boardsize_min1 = &command_list->possible_arguments.integer_boardsize_min1;
    command_argument_description_t *optional_string = &command_list->possible_arguments.optional_string;

    *generic_integer = _generic_integer;
    *generic_string = _generic_string;
    *zo_float = _zo_float;
    *boolean = _boolean;
    *integer_boardidx_min0 = _integer_boardidx_min0;
    *integer_boardidx_min1 = _integer_boardidx_min1;
    *integer_boardsize_min0 = _integer_boardsize_min0;
    *integer_boardsize_min1 = _integer_boardsize_min1;
    *optional_string = _optional_string;

    command_list->num_commands = NUM_COMMANDS;

    command_list->commands[0] = construct_command("solve", 1, generic_string, NULL, NULL, TRUE, TRUE, TRUE,
                                                  command_solve);
    command_list->commands[1] = construct_command("edit", 1, optional_string, NULL, NULL, TRUE, TRUE, TRUE,
                                                  command_edit);
    command_list->commands[2] = construct_command("mark_errors", 1, boolean, NULL, NULL, FALSE, FALSE, TRUE,
                                                  command_mark_errors);
    command_list->commands[3] = construct_command("print_board", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE,
                                                  command_print_board);
    command_list->commands[4] = construct_command("set", 3, integer_boardidx_min1, integer_boardidx_min1,
                                                  integer_boardidx_min0, FALSE, TRUE, TRUE, command_set);
    command_list->commands[5] = construct_command("validate", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE, command_validate);
    command_list->commands[6] = construct_command("guess", 1, zo_float, NULL, NULL, FALSE, FALSE, TRUE, command_guess);
    command_list->commands[7] = construct_command("generate", 2, integer_boardsize_min0, integer_boardsize_min1, NULL,
                                                  FALSE, TRUE, FALSE, command_generate);
    command_list->commands[8] = construct_command("undo", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE, command_undo);
    command_list->commands[9] = construct_command("redo", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE, command_redo);
    command_list->commands[10] = construct_command("save", 1, generic_string, NULL, NULL, FALSE, TRUE, TRUE,
                                                   command_save);
    command_list->commands[11] = construct_command("hint", 2, integer_boardidx_min1, integer_boardidx_min1, NULL, FALSE,
                                                   FALSE, TRUE, command_hint);
    command_list->commands[12] = construct_command("guess_hint", 2, integer_boardidx_min1, integer_boardidx_min1, NULL,
                                                   FALSE, FALSE, TRUE, command_guess_hint);
    command_list->commands[13] = construct_command("num_solutions", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE,
                                                   command_num_solutions);
    command_list->commands[14] = construct_command("autofill", 0, NULL, NULL, NULL, FALSE, FALSE, TRUE,
                                                   command_autofill);
    command_list->commands[15] = construct_command("reset", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE, command_reset);
    command_list->commands[16] = construct_command("exit", 0, NULL, NULL, NULL, TRUE, TRUE, TRUE, command_exit);
}

void update_arguments(command_list_t *command_list, const sudoku_game_t *game) {
    command_list->possible_arguments.integer_boardidx_min0.upper_bound.int_value = game->board->sub_board_size;
    command_list->possible_arguments.integer_boardidx_min1.upper_bound.int_value = game->board->sub_board_size;
    command_list->possible_arguments.integer_boardsize_min0.upper_bound.int_value = game->board->total_size;
    command_list->possible_arguments.integer_boardsize_min1.upper_bound.int_value = game->board->total_size;
}

/*
 * /==========\
 * | Commands |
 * \==========/
 */

enum command_status command_solve(sudoku_game_t *game, const command_arguments_t *args) {
    sudoku_board_t *board, *temp_board;
    enum load_file_status status;
    char *path;
    int row, col;

    path = args->arguments[0].value.str_value;
    status = board_from_file(path, &board);

    switch (status) {
        case LOAD_ENOFILE:
            printf("Error: No such file!\n");
            return CMD_ERR;
        case LOAD_EBADFILE:
            printf("Error: Bad file!\n");
            return CMD_ERR;
        default:
            break;
    }

    temp_board = create_board(board->rows, board->cols);
    copy_board(board, temp_board);

    for (row = 0; row < temp_board->total_rows; row++) {
        for (col = 0; col < temp_board->total_cols; col++) {
            if (get_cell_metadata_flattened(temp_board, row, col) != FIXED_METADATA) {
                set_cell_metadata_flattened(temp_board, EMPTY_METADATA, row, col);
                set_cell_flattened(temp_board, EMPTY_CELL, row, col);
            }
        }
    }

    if (!check_board(temp_board)) {
        printf("Error: Board is erroneous!\n");
        destruct_board(board);
        destruct_board(temp_board);
        return CMD_ERR;
    }

    destruct_board(temp_board);
    load_board(game, board);
    game->mode = SOLVE;
    return PARAMETER_UPDATE;
}

enum command_status command_edit(sudoku_game_t *game, const command_arguments_t *args) {
    sudoku_board_t *board;
    enum load_file_status status;
    char *path;

    if (args->num_arguments == 0) {
        board = create_board(DEFAULT_NUM_ROWS, DEFAULT_NUM_COlS);
    } else {
        path = args->arguments[0].value.str_value;
        status = board_from_file(path, &board);

        switch (status) {
            case LOAD_ENOFILE:
                printf("Error: No such file!\n");
                return CMD_ERR;
            case LOAD_EBADFILE:
                printf("Error: Bad file!\n");
                return CMD_ERR;
            default:
                break;
        }
    }

    load_board(game, board);
    game->mode = EDIT;
    return PARAMETER_UPDATE;
}

enum command_status command_mark_errors(sudoku_game_t *game, const command_arguments_t *args) {
    game->mark_errors = args->arguments[0].value.int_value;
    return DONE;
}

enum command_status command_print_board(sudoku_game_t *game, __attribute__ ((unused)) const command_arguments_t *args) {
    print_board(game->board, game->mode == EDIT || game->mark_errors, game->mode != EDIT);
    return DONE;
}

enum command_status command_set(sudoku_game_t *game, const command_arguments_t *args) {
    int row = args->arguments[0].value.int_value - 1, col = args->arguments[1].value.int_value - 1;
    int old_value = get_cell_flattened(game->board, row, col), new_value = args->arguments[2].value.int_value;
    sudoku_game_operation_t *operation;

    if (game->mode == SOLVE && get_cell_metadata_flattened(game->board, row, col) == FIXED_METADATA) {
        printf("Error: Can't edit fixed cell in solve mode!\n");
        return CMD_ERR;
    }

    operation = create_composite_game_operation(SET, row + 1, col + 1, new_value, 0);
    append_atomic_operation(operation, row, col, old_value, new_value);

    operation_list_delete_after(game->last_operation);
    if (operation_list_append(game->last_operation, operation))
        game->last_operation = game->last_operation->next;
    set_cell_flattened(game->board, new_value, row, col);
    return BOARD_UPDATE;
}

enum command_status command_validate(sudoku_game_t *game, __attribute__ ((unused)) const command_arguments_t *args) {
    int solvable;
    if (is_solvable(game->board, &solvable) != SUCCESS) {
        print_gurobi_error();
        return CMD_ERR;
    }

    if (solvable) {
        printf("Board has a solution!\n");
    } else {
        printf("Board has no solution!\n");
    }

    return DONE;
}

enum command_status command_guess(sudoku_game_t *game, const command_arguments_t *args) {
    printf("Unimplemented [%d, %d]!\n", game->mode, args->num_arguments);
    return BOARD_UPDATE;
}

enum command_status command_generate(sudoku_game_t *game, const command_arguments_t *args) {
    int x = args->arguments[0].value.int_value;
    /*int y = args->arguments[1].value.int_value;*/
    int iterations = 0;

    if (x > empty_cells_num(game->board)) {
        printf("Error: The board does not contain %d empty cells\n", x);
        return CMD_ERR;
    }

    if (!check_board(game->board)) {
        printf("Error: Board is erroneous!\n");
    }

    copy_board(game->board, game->temporary_board);

    while (iterations < MAX_ITERATIONS) {
        if (random_fill_empty_cells(game->board, x) != SUCCESS) {
            iterations++;
            continue;
        }

    }
    print_board(game->board, TRUE);


    return DONE;
    /*return BOARD_UPDATE;*/
}

enum command_status command_undo(sudoku_game_t *game, __attribute__ ((unused)) const command_arguments_t *args) {
    sudoku_game_operation_t *operation;

    if (game->last_operation->operation->operation_type != HEAD) {
        operation = game->last_operation->operation;
        game->last_operation = game->last_operation->prev;
        undo_operation(game, operation);

        print_game_operation(operation, TRUE);
        return BOARD_UPDATE;
    } else {
        printf("Error: no command to undo!\n");
        return CMD_ERR;
    }
}

enum command_status command_redo(sudoku_game_t *game, __attribute__ ((unused)) const command_arguments_t *args) {
    sudoku_game_operation_t *operation;

    if (game->last_operation->next != NULL) {
        game->last_operation = game->last_operation->next;
        operation = game->last_operation->operation;
        redo_operation(game, operation);

        print_game_operation(operation, FALSE);
        return BOARD_UPDATE;
    } else {
        printf("Error: no command to redo!\n");
        return CMD_ERR;
    }
}

enum command_status command_save(sudoku_game_t *game, const command_arguments_t *args) {
    char *path = args->arguments[0].value.str_value;
    int i, j;

    if (game->mode == EDIT) {
        if (!check_board(game->board)) {
            printf("Error: Erroneous board! Aborting save...\n");
            return CMD_ERR;
        }
        /* TODO: Replace with cheaper alternative? */
        if (backtracking(game->board) <= 0) {
            printf("Error: Board has not solution! Aborting save...\n");
            return CMD_ERR;
        }
    }

    copy_board(game->board, game->temporary_board);
    for (i = 0; i < game->board->total_rows; i++) {
        for (j = 0; j < game->board->total_cols; j++) {
            if (get_cell_flattened(game->temporary_board, i, j) == EMPTY_CELL)
                set_cell_metadata_flattened(game->temporary_board, EMPTY_METADATA, i, j);
            else
                set_cell_metadata_flattened(game->temporary_board, FIXED_METADATA, i, j);
        }
    }
    if (save_board_to_file(path, game->temporary_board) != SUCCESS) {
        printf("Error: Encountered an error while saving board to file! [err=%s]\n", strerror(errno));
        return CMD_ERR;
    }

    return DONE;
}

enum command_status command_hint(sudoku_game_t *game, const command_arguments_t *args) {
    int row = args->arguments[0].value.int_value - 1;
    int col = args->arguments[1].value.int_value - 1;
    int ret_val;

    if (!check_board(game->board)) {
        printf("Error: Erroneous board!\n");
        return CMD_ERR;
    }
    if (get_cell_metadata_flattened(game->board, row, col) == FIXED_METADATA) {
        printf("Error: The cell is fixed!\n");
        return CMD_ERR;
    }
    if (get_cell_flattened(game->board, row, col) != 0) {
        printf("Error: Cell already contains a value!\n");
        return CMD_ERR;
    }
    copy_board(game->board, game->temporary_board);

    ret_val = solve_board(game->temporary_board, ILP);
    if (ret_val == ERROR) {
        print_gurobi_error();
        return CMD_ERR;
    } else if (ret_val == UNSOLVABLE) {
        printf("Error: The board is unsolvable.");
        return CMD_ERR;
    }
    printf("Hint: try setting %d, %d to %d\n", row + 1, col + 1, get_cell_flattened(game->temporary_board, row, col));

    return DONE;
}

enum command_status command_guess_hint(sudoku_game_t *game, const command_arguments_t *args) {
    printf("Unimplemented [%d, %d]!\n", game->mode, args->num_arguments);
    return DONE;
}

enum command_status
command_num_solutions(sudoku_game_t *game, __attribute__ ((unused)) const command_arguments_t *args) {
    int num_solutions = backtracking(game->board);
    printf("Current board has [%d] solution(s).\n", num_solutions);
    return DONE;
}

enum command_status command_autofill(sudoku_game_t *game, __attribute__((unused)) const command_arguments_t *args) {
    int i, j, v, f, t;
    if (check_board(game->board)) {
        printf("Error: Board is erroneous!\n");
        return CMD_ERR;
    }

    copy_board(game->board, game->temporary_board);
    for (i = 0; i < game->board->total_rows; i++) {
        for (j = 0; j < game->board->total_cols; j++) {
            if (get_cell_flattened(game->board, i, j) != EMPTY_CELL)
                continue;

            f = 0, v = 0;
            for(t = 1; t <= game->board->sub_board_size; t++) {
                if (check_value_flattened(game->board, t, i, j)) {
                    v = t;
                    f++;
                }
            }

            if (f == 1)
                set_cell_flattened(game->temporary_board, v, i, j);
        }
    }
    copy_board(game->temporary_board, game->board);
    return BOARD_UPDATE;
}

enum command_status command_reset(sudoku_game_t *game, __attribute__ ((unused)) const command_arguments_t *args) {
    sudoku_game_operation_t *operation;

    while (game->last_operation->operation->operation_type != HEAD) {
        operation = game->last_operation->operation;
        game->last_operation = game->last_operation->prev;
        undo_operation(game, operation);
    }

    return BOARD_UPDATE;
}

enum command_status
command_exit(__attribute__ ((unused)) sudoku_game_t *game, __attribute__ ((unused)) const command_arguments_t *args) {
    return EXIT_PROGRAM;
}

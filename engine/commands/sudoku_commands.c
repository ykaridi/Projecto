#include <errno.h>
#include <string.h>
#include "sudoku_commands.h"
#include "../fs/sudoku_fs.h"
#include "../sudoku_solver.h"

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
enum command_status command_exit(sudoku_game_t *game, const command_arguments_t *args) ;

command_t construct_command(const char* name, int num_args, command_argument_description_t *arg1,
                            command_argument_description_t *arg2, command_argument_description_t *arg3, int available_init,
                            int available_edit, int available_solve,
                            enum command_status (*function)(sudoku_game_t *, const command_arguments_t *)) {
    command_t command;
    command_arguments_description_t* arguments = &command.args;
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
            INTEGER, FALSE, FALSE, -1, -1
    };
    command_argument_description_t _generic_string = {
            STRING, FALSE, FALSE, -1, -1
    };
    command_argument_description_t _zo_float = {
            FLOAT, FALSE, FALSE, 0, 1
    };
    command_argument_description_t _boolean = {
            INTEGER, FALSE, TRUE, 0, 1
    };
    command_argument_description_t _integer_boardidx_min0 = {
            INTEGER, FALSE, TRUE, 0, -1
    };
    command_argument_description_t _integer_boardidx_min1 = {
            INTEGER, FALSE, TRUE, 1, -1
    };
    command_argument_description_t _integer_boardsize_min0 = {
            INTEGER, FALSE, TRUE, 1, -1
    };
    command_argument_description_t _integer_boardsize_min1 = {
            INTEGER, FALSE, TRUE, 1, -1
    };
    command_argument_description_t _optional_string = {
            STRING, TRUE, FALSE, -1, -1
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

    command_list->commands[0] = construct_command("solve", 1, generic_string, NULL, NULL, TRUE, TRUE, TRUE, command_solve);
    command_list->commands[1] = construct_command("edit", 1, optional_string, NULL, NULL, TRUE, TRUE, TRUE, command_edit);
    command_list->commands[2] = construct_command("mark_errors", 1, boolean, NULL, NULL, FALSE, FALSE, TRUE, command_mark_errors);
    command_list->commands[3] = construct_command("print_board", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE, command_print_board);
    command_list->commands[4] = construct_command("set", 3, integer_boardidx_min1, integer_boardidx_min1, integer_boardidx_min0, FALSE, TRUE, TRUE, command_set);
    command_list->commands[5] = construct_command("validate", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE, command_validate);
    command_list->commands[6] = construct_command("guess", 1, zo_float, NULL, NULL, FALSE, FALSE, TRUE, command_guess);
    command_list->commands[7] = construct_command("generate", 2, integer_boardsize_min0, integer_boardsize_min1, NULL, FALSE, TRUE, FALSE, command_generate);
    command_list->commands[8] = construct_command("undo", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE, command_undo);
    command_list->commands[9] = construct_command("redo", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE, command_redo);
    command_list->commands[10] = construct_command("save", 1, generic_string, NULL, NULL, FALSE, TRUE, TRUE, command_save);
    command_list->commands[11] = construct_command("hint", 2, integer_boardidx_min1, integer_boardidx_min1, NULL, FALSE, FALSE, TRUE, command_hint);
    command_list->commands[12] = construct_command("guess_hint", 2, integer_boardidx_min1, integer_boardidx_min1, NULL, FALSE, FALSE, TRUE, command_guess_hint);
    command_list->commands[13] = construct_command("num_solutions", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE, command_num_solutions);
    command_list->commands[14] = construct_command("autofill", 0, NULL, NULL, NULL, FALSE, FALSE, TRUE, command_autofill);
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
    return BOARD_UPDATE;
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
                printf("Error: No such file!");
                return CMD_ERR;
            case LOAD_EBADFILE:
                printf("Error: Bad file!");
                return CMD_ERR;
            default:
                break;
        }
    }

    load_board(game, board);
    game->mode = EDIT;
    return BOARD_UPDATE;
}

enum command_status command_mark_errors(sudoku_game_t *game, const command_arguments_t *args) {
    game->mark_errors = args->arguments[0].value.int_value;
    return DONE;
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
enum command_status command_print_board(sudoku_game_t *game, const command_arguments_t *args) {
    /* TODO: Treat mark errors */
    print_board(game->board);
    return DONE;
}

enum command_status command_set(sudoku_game_t *game, const command_arguments_t *args) {
    int row = args->arguments[0].value.int_value - 1, col = args->arguments[1].value.int_value - 1;
    int value = args->arguments[2].value.int_value;

    if (game->mode == SOLVE && get_cell_metadata_flattened(game->board, row, col) == FIXED_METADATA) {
        printf("Error: Can't edit fixed cell in solve mode!\n");
        return CMD_ERR;
    }

    set_cell_flattened(game->board, value, row, col);
    return DONE;
}

enum command_status command_validate(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_guess(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_generate(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_undo(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_redo(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_save(sudoku_game_t *game, const command_arguments_t *args) {
    char *path = args->arguments[0].value.str_value;

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

    if (save_board_to_file(path, game->board) != SUCCESS) {
        printf("Error: Encountered an error while saving board to file! [err=%s]", strerror(errno));
        return CMD_ERR;
    }

    return DONE;
}

enum command_status command_hint(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_guess_hint(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_num_solutions(sudoku_game_t *game, const command_arguments_t *args) {
    int num_solutions = backtracking(game->board);
    printf("Current board has [%d] solutions.", num_solutions);
    return DONE;
}

enum command_status command_autofill(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_reset(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_exit(sudoku_game_t *game, const command_arguments_t *args) {
    return EXIT_PROGRAM;
}

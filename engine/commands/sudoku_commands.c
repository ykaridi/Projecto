#include "sudoku_commands.h"

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
    return DONE;
}

enum command_status command_edit(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_mark_errors(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_print_board(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_set(sudoku_game_t *game, const command_arguments_t *args) {
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
    return DONE;
}

enum command_status command_hint(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_guess_hint(sudoku_game_t *game, const command_arguments_t *args) {
    return DONE;
}

enum command_status command_num_solutions(sudoku_game_t *game, const command_arguments_t *args) {
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

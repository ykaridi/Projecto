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
    command_arguments_description_t arguments;
    modes_availability_t modes_of_availability;

    arguments.num_arguments = num_args;
    arguments.arguments[0] = arg1;
    arguments.arguments[1] = arg2;
    arguments.arguments[2] = arg3;

    modes_of_availability.init = available_init;
    modes_of_availability.edit = available_edit;
    modes_of_availability.solve = available_solve;

    return (command_t) {
            name, arguments, modes_of_availability, function
    };
}

void load_commands(command_list_t *command_list) {
    int i = 0;
    command_argument_description_t *generic_integer = &command_list->possible_arguments.generic_integer;
    command_argument_description_t *generic_string = &command_list->possible_arguments.generic_string;
    command_argument_description_t *zo_float = &command_list->possible_arguments.zo_float;
    command_argument_description_t *boolean = &command_list->possible_arguments.boolean;
    command_argument_description_t *integer_boardidx_min0 = &command_list->possible_arguments.integer_boardidx_min0;
    command_argument_description_t *integer_boardidx_min1 = &command_list->possible_arguments.integer_boardidx_min1;
    command_argument_description_t *integer_boardsize_min0 = &command_list->possible_arguments.integer_boardsize_min0;
    command_argument_description_t *integer_boardsize_min1 = &command_list->possible_arguments.integer_boardsize_min1;
    command_argument_description_t *optional_string = &command_list->possible_arguments.optional_string;
    command_t commands[17] = {
            construct_command("solve", 1, generic_string, NULL, NULL, TRUE, TRUE, TRUE,
                              command_solve),
            construct_command("edit", 1, optional_string, NULL, NULL, TRUE, TRUE, TRUE,
                              command_edit),
            construct_command("mark_errors", 1, boolean, NULL, NULL, FALSE, FALSE, TRUE,
                              command_mark_errors),
            construct_command("print_board", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE,
                              command_print_board),
            construct_command("set", 3, integer_boardidx_min1, integer_boardidx_min1, integer_boardidx_min0, FALSE, TRUE, TRUE,
                              command_set),
            construct_command("validate", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE,
                              command_validate),
            construct_command("guess", 1, zo_float, NULL, NULL, FALSE, FALSE, TRUE,
                              command_guess),
            construct_command("generate", 2, integer_boardsize_min0, integer_boardsize_min1, NULL, FALSE, TRUE, FALSE,
                              command_generate),
            construct_command("undo", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE,
                              command_undo),
            construct_command("redo", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE,
                              command_redo),
            construct_command("save", 1, generic_string, NULL, NULL, FALSE, TRUE, TRUE,
                              command_save),
            construct_command("hint", 2, integer_boardidx_min1, integer_boardidx_min1, NULL, FALSE, FALSE, TRUE,
                              command_hint),
            construct_command("guess_hint", 2, integer_boardidx_min1, integer_boardidx_min1, NULL, FALSE, FALSE, TRUE,
                              command_guess_hint),
            construct_command("num_solutions", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE,
                              command_num_solutions),
            construct_command("autofill", 0, NULL, NULL, NULL, FALSE, FALSE, TRUE,
                              command_autofill),
            construct_command("reset", 0, NULL, NULL, NULL, FALSE, TRUE, TRUE,
                              command_reset),
            construct_command("exit", 0, NULL, NULL, NULL, TRUE, TRUE, TRUE,
                              command_exit)
    };

    *generic_integer = (command_argument_description_t) {
            INTEGER, FALSE, FALSE, -1, -1
    };
    *generic_string = (command_argument_description_t) {
            STRING, FALSE, FALSE, -1, -1
    };
    *zo_float = (command_argument_description_t) {
            FLOAT, FALSE, FALSE, 0, 1
    };
    *boolean = (command_argument_description_t) {
            INTEGER, FALSE, TRUE, 0, 1
    };
    *integer_boardidx_min0 = (command_argument_description_t) {
            INTEGER, FALSE, TRUE, 0, -1
    };
    *integer_boardidx_min1 = (command_argument_description_t) {
            INTEGER, FALSE, TRUE, 1, -1
    };
    *integer_boardsize_min0 = (command_argument_description_t) {
            INTEGER, FALSE, TRUE, 1, -1
    };
    *integer_boardsize_min1 = (command_argument_description_t) {
            INTEGER, FALSE, TRUE, 1, -1
    };
    *optional_string = (command_argument_description_t) {
            STRING, TRUE, FALSE, -1, -1
    };

    command_list->num_commands = NUM_COMMANDS;
    for (i = 0; i < NUM_COMMANDS; i++) {
        command_list->commands[i] = commands[i];
    }
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
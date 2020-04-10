#include "grb_commands.h"
#include "grb_processing.h"
#include "grb_setup.h"

#define CLEANUP_AND_RETURN {GRBfreemodel(model); \
                            free_var_list(var_list); \
                            return GRB_ERROR; }

/**
 * Create a grb_solve argument.
 */
void create_grb_solve(grb_args_t *args, bool *solvable_ptr) {
    args->command = GRB_SOLVE;
    args->solvable_ptr = solvable_ptr;
}

/**
 * Create a grb_validate argument.
 */
void create_grb_validate(grb_args_t *args, bool *solvable_ptr) {
    args->command = GRB_VALIDATE;
    args->solvable_ptr = solvable_ptr;
}

/**
 * Create a grb_guess argument.
 */
void create_grb_guess(grb_args_t *args, bool *solvable_ptr, float threshold) {
    args->command = GRB_GUESS;
    args->solvable_ptr = solvable_ptr;
    args->input.guess_value = threshold;
}

/**
 * Create a grb_hint argument.
 */
void create_grb_hint(grb_args_t *args, bool *solvable_ptr, int row, int col, int *value) {
    args->command = GRB_HINT;
    args->solvable_ptr = solvable_ptr;
    args->input.hint_value.row = row;
    args->input.hint_value.col = col;
    args->input.hint_value.value = value;
}

/**
 * Create a grb_guess_hint argument.
 */
void create_grb_guess_hint(grb_args_t *args, bool *solvable_ptr, int row, int col, int *values, int *len) {
    args->command = GRB_GUESS_HINT;
    args->solvable_ptr = solvable_ptr;
    args->input.guess_hint_value.row = row;
    args->input.guess_hint_value.col = col;
    args->input.guess_hint_value.values = values;
    args->input.guess_hint_value.len = len;
}

/**
 * Checks if the command needs to process only one cell in the given board.
 */
bool is_lp(enum grb_command command) {
    return command == GRB_GUESS || command == GRB_GUESS_HINT;
}

/**
 * Runs a command that uses gurobi on the given board.
 * @param board
 * @param command
 * @return
 */
enum grb_output grb_command(GRBenv **env, sudoku_board_t *board, const grb_args_t *args) {
    GRBmodel *model = NULL;
    var_list_t *var_list = NULL;
    int row, col;
    float threshold = 0;
    bool solvable;
    single_cell_input_t single_cell_input;
    enum programming_type type = is_lp(args->command) ? LP : ILP;

    /* Setting up the enviroment, model and var_list. */
    if (get_enviroment(env) != SUCCESS) {
        return GRB_ERROR;
    }

    if (init_model(*env, &model) != SUCCESS) {
        return GRB_ERROR;
    }

    var_list = get_variables(board);

    /* Running Gurobi on the board with the given programming type (ILP/LP). */
    if (run_gurobi_on_board(model, var_list, type) != SUCCESS) {
        CLEANUP_AND_RETURN
    }

    /* Checking if the result is solvable. */
    if (optimized_model_is_solvable(model, &solvable) != SUCCESS) {
        CLEANUP_AND_RETURN
    }
    *(args->solvable_ptr) = solvable;

    /* Processing the output of the optimizer, according to the command. */
    if (solvable) {
        switch (args->command) {
            /* GUESS is same as SOLVE, only we have to set a threshold. */
            case GRB_GUESS:
                threshold = args->input.guess_value;
                /* fall through */
            case GRB_SOLVE:
                /* Get the solved board from the model. */
                if (get_solved_board(model, var_list, board, type, threshold) != SUCCESS) {
                    CLEANUP_AND_RETURN
                }
                break;
            case GRB_HINT:
                /* We only want a single cell out of the model, so we use a different function,
                 * setting the input to be ILP. */
                row = args->input.hint_value.row;
                col = args->input.hint_value.col;
                create_single_cell_input_ilp(&single_cell_input, args->input.hint_value.value);

                if (process_single_cell(model, var_list->cell_vars[row][col], &single_cell_input) != SUCCESS) {
                    CLEANUP_AND_RETURN
                }
                break;
            case GRB_GUESS_HINT:
                /* We only want a single cell out of the model, so we use a different function,
                 * setting the input to be LP. */
                row = args->input.guess_hint_value.row;
                col = args->input.guess_hint_value.col;
                create_single_cell_input_lp(&single_cell_input, args->input.guess_hint_value.values,
                                            args->input.guess_hint_value.len);

                if (process_single_cell(model, var_list->cell_vars[row][col], &single_cell_input) != SUCCESS) {
                    CLEANUP_AND_RETURN
                }
                break;
            case GRB_VALIDATE:
                break;
        }
    }

    GRBfreemodel(model);
    free_var_list(var_list);
    return GRB_SUCCESS;
}

/**
 * Prints the last gurobi error.
 */
void print_gurobi_error(GRBenv* env) {
    printf("Error: Gurobi: %s\n", GRBgeterrormsg(env));
}

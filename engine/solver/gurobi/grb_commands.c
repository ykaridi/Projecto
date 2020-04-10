#include "grb_commands.h"
#include "sudoku_grb.h"

#define CLEANUP_AND_RETURN {GRBfreemodel(model); \
                            free_var_list(var_list); \
                            return GRB_ERROR; }


void create_grb_solve(grb_args_t *args, bool *solvable_ptr) {
    args->command = GRB_SOLVE;
    args->solvable_ptr = solvable_ptr;
}

void create_grb_validate(grb_args_t *args, bool *solvable_ptr) {
    args->command = GRB_VALIDATE;
    args->solvable_ptr = solvable_ptr;
}

void create_grb_guess(grb_args_t *args, bool *solvable_ptr, float threshold) {
    args->command = GRB_GUESS;
    args->solvable_ptr = solvable_ptr;
    args->input.guess_value = threshold;
}

void create_grb_hint(grb_args_t *args, bool *solvable_ptr, int row, int col, int *value) {
    args->command = GRB_HINT;
    args->solvable_ptr = solvable_ptr;
    args->input.hint_value.row = row;
    args->input.hint_value.col = col;
    args->input.hint_value.value = value;
}

void create_grb_guess_hint(grb_args_t *args, bool *solvable_ptr, int row, int col, int *values, int *len) {
    args->command = GRB_GUESS_HINT;
    args->solvable_ptr = solvable_ptr;
    args->input.guess_hint_value.row = row;
    args->input.guess_hint_value.col = col;
    args->input.guess_hint_value.values = values;
    args->input.guess_hint_value.len = len;
}


/**
 * Runs a command that uses gurobi on the given board.
 * @param board
 * @param command
 * @return
 */
enum grb_output grb_command(sudoku_board_t *board, const grb_args_t *args) {
    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    var_list_t *var_list = NULL;
    int row, col;
    float threshold = 0;
    bool solvable;
    enum programming_type type = (args->command == GRB_GUESS || args->command == GRB_GUESS_HINT) ? LP : ILP;

    if (get_enviroment(&env) != SUCCESS) {
        return GRB_ERROR;
    }

    if (init_model(env, &model) != SUCCESS) {
        return GRB_ERROR;
    }

    var_list = get_variables(board);

    if (run_gurobi_on_board(model, var_list, type) != SUCCESS) {
        CLEANUP_AND_RETURN
    }

    if (optimized_model_is_solvable(model, &solvable) != SUCCESS) {
        CLEANUP_AND_RETURN
    }
    *(args->solvable_ptr) = solvable;

    if (solvable) {
        switch (args->command) {
            case GRB_GUESS:
                threshold = args->input.guess_value;
                /* fall through */
            case GRB_SOLVE:
                if (get_solved_board(model, var_list, board, type, threshold) != SUCCESS) {
                    CLEANUP_AND_RETURN
                }
                break;
            case GRB_HINT:
                row = args->input.hint_value.row;
                col = args->input.hint_value.col;
                if (get_single_cell_ilp(model, var_list->cell_vars[row][col], args->input.hint_value.value) !=
                    SUCCESS) {
                    CLEANUP_AND_RETURN
                }
                break;
            case GRB_GUESS_HINT:
                row = args->input.guess_hint_value.row;
                col = args->input.guess_hint_value.col;
                if (get_single_cell_lp(model, var_list->cell_vars[row][col], args->input.guess_hint_value.values,
                                       args->input.guess_hint_value.len) != SUCCESS) {
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
void print_gurobi_error() {
    GRBenv *env = NULL;
    if (!get_enviroment(&env)) {
        printf("Error: Gurobi: Could not get environment.\n");
        return;
    }
    printf("Error: Gurobi: %s\n", GRBgeterrormsg(env));
}

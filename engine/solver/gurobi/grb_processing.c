#include <stdlib.h>
#include <stdio.h>

#include "grb_processing.h"
#include "../../../utils.h"
#include "../../sudoku_board.h"
#include "sudoku_var_list.h"
#include "../../random/random.h"
#include "grb_setup.h"


/**
 * Checks if an already optimized model is solvable.
 * @param model
 * @param solvable
 * @return SUCCESS/ERROR
 */
int optimized_model_is_solvable(GRBmodel *model, bool *solvable) {
    int optimstatus;

    if (GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus)) {
        return ERROR;
    }

    if (optimstatus == GRB_OPTIMAL) {
        *solvable = true;
    } else {
        *solvable = false;
    }
    return SUCCESS;
}


/**
 * returns value that needs to be put in the cell_lists' cell, after running ILP.
 * @param cell_list
 * @param outputs
 * @return
 */
int get_value_from_outputs_ilp(const cell_var_list_t *cell_list, const double *outputs) {
    int v, size = cell_list->max_size, var = 0;

    for (v = 0; v < size; v++) {
        if (cell_list->vars[v].taken) {
            if (outputs[var++] == 1) {
                return v + 1;
            }
        }
    }
    return 0;
}

/**
 * Creates an input fot the process_single_cell function, where the programming type is ILP.
 */
void create_single_cell_input_ilp(single_cell_input_t* single_cell_input, int* ilp_value) {
    single_cell_input->type = ILP;
    single_cell_input->input.ilp_value = ilp_value;
}

/**
 * Creates an input fot the process_single_cell function, where the programming type is LP.
 */
void create_single_cell_input_lp(single_cell_input_t* single_cell_input, int* values, int* len) {
    single_cell_input->type = LP;
    single_cell_input->input.lp_value.values = values;
    single_cell_input->input.lp_value.len = len;
}


/**
 * processes a single cell output from an optimized model, depending on the programming type (ILP/LP).
 * For LP - Outputs all numbers which have weights > 0.
 * For ILP - Outputs the single number which has weight 1.
 * Assumes that there is a solution to the model.
 */
int process_single_cell(GRBmodel *model, const cell_var_list_t *cell_var_list, single_cell_input_t* input) {
    int v;
    double *outputs = malloc(cell_var_list->max_size * sizeof(double));

    if (!outputs) {
        EXIT_ON_ERROR("malloc")
    }

    for (v = 0; v < cell_var_list->max_size; ++v) {
        if (cell_var_list->vars[v].taken) {
            break;
        }
    }

    /* After this output will contain the values of the variables in the cell. */
    if (GRBgetdblattrarray(model, GRB_DBL_ATTR_X, cell_var_list->vars[v].index, cell_var_list->num_vars, outputs)) {
        free(outputs);
        return ERROR;
    }

    if (input->type == ILP) {
        /* We already have a function for this. */
        *(input->input.ilp_value) = get_value_from_outputs_ilp(cell_var_list, outputs);
    } else { /* type == LP */
        int len = 0, output_index = 0;
        for (v = 0; v < cell_var_list->max_size; v++) {
            if (cell_var_list->vars[v].taken) {
                if (outputs[output_index++] > 0) {
                    input->input.lp_value.values[len++] = v;
                }
            }
        }
        *input->input.lp_value.len = len;
    }


    free(outputs);
    return SUCCESS;
}

/**
 * returns value that needs to be put in the cell_lists' cell, after running LP.
 * @param cell_list
 * @param board
 * @param outputs
 * @param row
 * @param col
 */
int get_value_from_outputs_lp(const cell_var_list_t *cell_list, sudoku_board_t *board, const double *outputs, int row,
                              int col,
                              float threshold) {
    int v, size = cell_list->max_size, var = 0, possible_outputs_len = 0;
    var_weight_t *possible_outputs = malloc(cell_list->num_vars * sizeof(var_weight_t));
    double weight_sum = 0;
    if (!possible_outputs) {
        EXIT_ON_ERROR("malloc")
    }

    for (v = 0; v < size; v++) {
        if (cell_list->vars[v].taken) {
            /* TODO: change this to O(n) */
            if (outputs[var] >= threshold && check_value_flattened(board, v + 1, row, col)) {
                possible_outputs[possible_outputs_len].var = v;
                possible_outputs[possible_outputs_len].weight = outputs[var];
                possible_outputs_len++;
                weight_sum += outputs[var];
            }
            var++;
        }
    }
    if (possible_outputs_len > 0) {
        v = choose_var(possible_outputs, possible_outputs_len, weight_sum) + 1;
    } else {
        v = 0;
    }

    free(possible_outputs);
    return v;
}


/**
 * Gets an optimized model and var_list and sets the board according to the solution.
 * @param model
 * @param var_list
 * @param board
 * @param type
 * @param threshold This is ignored for ILP mode.
 * @return
 */
int get_solved_board(GRBmodel *model, const var_list_t *var_list, sudoku_board_t *board,
                     enum programming_type type, float threshold) {
    int i, j, index = 0, size = var_list->size, v;
    double *outputs;


    outputs = malloc(size * sizeof(double));
    if (!outputs) {
        EXIT_ON_ERROR("malloc")
    }

    for (i = 0; i < board->total_rows; ++i) {
        for (j = 0; j < board->total_cols; ++j) {
            cell_var_list_t *cell_list = var_list->cell_vars[i][j];
            if (cell_list->num_vars == 0)
                continue;

            if (GRBgetdblattrarray(model, GRB_DBL_ATTR_X, index, cell_list->num_vars, outputs)) {
                free(outputs);
                return ERROR;
            }
            index += cell_list->num_vars;
            if (type == ILP) {
                v = get_value_from_outputs_ilp(cell_list, outputs);
            } else {
                v = get_value_from_outputs_lp(cell_list, board, outputs, i, j, threshold);
            }
            set_cell_flattened(board, v, i, j);
        }
    }

    free(outputs);
    return SUCCESS;
}

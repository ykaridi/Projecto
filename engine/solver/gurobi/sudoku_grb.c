#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sudoku_grb.h"
#include "gurobi_c.h"
#include "../../../utils.h"
#include "../../sudoku_board.h"
#include "sudoku_var_list.h"


/** Gets the gurobi environment.
 * @param env_ptr
 * @return SUCCESS/ERROR value. env_ptr can change on error.
 */
int get_enviroment(GRBenv **env_ptr) {
    static GRBenv *environment = NULL;
    if (environment != NULL) {
        *env_ptr = environment;
        return SUCCESS;
    }

    if (GRBemptyenv(&environment)) {
        return ERROR;
    }
    /* Set the log file */
    if (GRBsetstrparam(environment, "LogFile", LOG_FILE)) {
        GRBfreeenv(environment);
        return ERROR;
    }
    /* Start the enviroment */
    if (GRBstartenv(environment)) {
        GRBfreeenv(environment);
        return ERROR;
    }

    if (!GURUBI_OUTPUT) {
        if (GRBsetintparam(environment, "OutputFlag", 0)) {
            GRBfreeenv(environment);
            return ERROR;
        }
    }

    *env_ptr = environment;
    return SUCCESS;
}

/**
 * Create an empty model corresponding to the given environment.
 * @param env
 * @param model
 * @return SUCCESS/ERROR
 */
int init_model(GRBenv *env, GRBmodel **model) {
    if (GRBnewmodel(env, model, DEFAULT_MODEL_NAME, 0, NULL, NULL, NULL, NULL, NULL)) {
        return ERROR;
    }
    return SUCCESS;
}

/**
 * turn the board to the variables needed.
 * @param board
 * @param var_list_ptr
 * @return the variables.
 */
var_list_t *get_variables(sudoku_board_t *board) {
    int m = board->rows, n = board->cols, i, j, k, ctr = 0;
    int N = m * n;
    var_list_t *var_list = init_var_list(m, n);

    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            if (get_cell_flattened(board, i, j) != 0) {
                continue;
            }
            for (k = 0; k < N; ++k) {
                if (check_value_flattened(board, k + 1, i, j)) {
                    var_list_add_var(var_list, i, j, k, ctr++);
                }
            }
        }
    }
    return var_list;
}

/**
 * Add variables to the model given the var_list.
 * @param model
 * @param var_list
 * @param prog_type
 * @return
 */
int add_variables(GRBmodel *model, const var_list_t *var_list, enum programming_type prog_type) {
    int i, j, size = var_list->size;
    double *obj;
    char *vtype;
    double *lb = NULL, *ub = NULL;
    char type;

    /* Setup the variable_t types and the objective function. */
    if (prog_type == ILP)
        type = GRB_BINARY;
    else {
        type = GRB_CONTINUOUS;
        lb = malloc(size * sizeof(double));
        ub = malloc(size * sizeof(double));
        if (!lb || !ub) {
            EXIT_ON_ERROR("malloc")
        }
        for (i = 0; i < size; ++i) {
            lb[i] = 0.0;
            ub[i] = 1.0;
        }
    }
    vtype = malloc(size * sizeof(char));
    if (!vtype) {
        EXIT_ON_ERROR("malloc")
    }
    obj = malloc(size * sizeof(double));
    if (!obj) {
        EXIT_ON_ERROR("malloc")
    }
    for (i = 0; i < size; ++i) {
        obj[i] = 1;
        vtype[i] = type;
    }

    /* Adding the variables */
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            cell_var_list_t *cell_list = var_list->cell_vars[i][j];
            if (GRBaddvars(model, cell_list->num_vars, 0, NULL, NULL, NULL, obj,
                           lb, ub, vtype, NULL)) {
                free(obj);
                free(vtype);
                free(lb);
                free(ub);
                return ERROR;
            }
        }
    }
    free(obj);
    free(vtype);
    free(lb);
    free(ub);
    return SUCCESS;
}


/**
 * Adding constraints after the variables are added.
 * @param model
 * @param var_list
 * @return
 */
int add_constraints(GRBmodel *model, const var_list_t *var_list) {
    int i, j, k, sub_board_i, sub_board_j, v, m = var_list->rows, n = var_list->cols, size = var_list->size;
    int *ind = NULL;
    double *val = NULL;

    ind = malloc(size * sizeof(int));
    if (!ind) {
        EXIT_ON_ERROR("malloc")
    }
    val = malloc(size * sizeof(double));
    if (!val) {
        EXIT_ON_ERROR("malloc")
    }
    for (i = 0; i < size; ++i) {
        val[i] = 1;
    }

    /* Add cell constraints */
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            cell_var_list_t *list = var_list->cell_vars[i][j];
            int len = list->num_vars;
            int l = 0;

            if (len == 0) {
                continue;
            }

            for (k = 0; k < size && l < len; k++) {
                if (list->vars[k].taken)
                    ind[l++] = list->vars[k].index;
            }
            if (GRBaddconstr(model, len, ind, val, GRB_EQUAL, 1.0, NULL)) {
                free(ind);
                free(val);
                return ERROR;
            }
        }
    }

    /* Adding row constraints */
    for (v = 0; v < size; v++) {
        for (i = 0; i < size; ++i) {
            int len = 0;
            for (j = 0; j < size; ++j) {
                cell_var_list_t *list = var_list->cell_vars[i][j];
                if (list->num_vars == 0)
                    continue;

                if (list->vars[v].taken) {
                    ind[len++] = list->vars[v].index;
                }
            }
            if (len == 0)
                continue;
            if (GRBaddconstr(model, len, ind, val, GRB_EQUAL, 1.0, NULL)) {
                free(ind);
                free(val);
                return ERROR;
            }
        }
    }

    /* Adding column constraints */
    for (v = 0; v < size; v++) {
        for (j = 0; j < size; ++j) {
            int len = 0;
            for (i = 0; i < size; ++i) {
                cell_var_list_t *list = var_list->cell_vars[i][j];
                if (list->num_vars == 0)
                    continue;

                if (list->vars[v].taken) {
                    ind[len++] = list->vars[v].index;
                }
            }
            if (len == 0)
                continue;
            if (GRBaddconstr(model, len, ind, val, GRB_EQUAL, 1.0, NULL)) {
                free(ind);
                free(val);
                return ERROR;
            }
        }
    }

    /* Adding sub board constraints */
    for (v = 0; v < size; v++) {
        for (i = 0; i < var_list->cols; ++i) {
            for (j = 0; j < var_list->rows; ++j) {
                int len = 0;
                for (sub_board_i = 0; sub_board_i < var_list->rows; ++sub_board_i) {
                    for (sub_board_j = 0; sub_board_j < var_list->cols; ++sub_board_j) {
                        cell_var_list_t *list = var_list->cell_vars[i * m + sub_board_i][j * n + sub_board_j];
                        if (list->num_vars == 0)
                            continue;
                        if (list->vars[v].taken) {
                            ind[len++] = list->vars[v].index;
                        }
                    }
                }
                if (len == 0)
                    continue;
                if (GRBaddconstr(model, len, ind, val, GRB_EQUAL, 1.0, NULL)) {
                    free(ind);
                    free(val);
                    return ERROR;
                }
            }
        }
    }

    free(ind);
    free(val);
    return SUCCESS;
}


/**
 * setup the variables and constraints from the given board.
 * @param model
 * @param board
 * @param type
 * @return SUCCESS/ERROR
 */
int setup_model(GRBmodel *model, var_list_t *var_list, enum programming_type prog_type) {

    if (add_variables(model, var_list, prog_type) != SUCCESS) {
        free_var_list(var_list);
        GRBfreemodel(model);
        return ERROR;
    }

    if (GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE)) {
        free_var_list(var_list);
        GRBfreemodel(model);
        return ERROR;
    }

    if (add_constraints(model, var_list) != SUCCESS) {
        free_var_list(var_list);
        GRBfreemodel(model);
        return ERROR;
    }

    return SUCCESS;
}

/**
 * Gets an optimized model and var_list and sets the board according to the solution.
 * @param model
 * @param var_list
 * @param board
 * @return
 */
int get_solved_board_ilp(GRBmodel *model, const var_list_t *var_list, sudoku_board_t *board) {
    int i, j, v, var, index = 0, size = var_list->size;
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
            var = 0;
            for (v = 0; v < size; v++) {
                if (cell_list->vars[v].taken) {
                    if (outputs[var++] == 1) {
                        set_cell_flattened(board, v + 1, i, j);
                        break;
                    }
                }
            }
        }
    }

    free(outputs);
    return SUCCESS;
}

/**
 * Runs the ILP/LP Gurobi optimizer on the given board.
 * @param model
 * @param board
 * @param type
 * @return SUCCESS/ERROR.
 */
int run_gurobi_on_board(GRBmodel *model, var_list_t* var_list, enum programming_type type) {
    if (setup_model(model, var_list, type) != SUCCESS) {
        return ERROR;
    }

    if (GRBoptimize(model)) {
        return ERROR;
    }

    return SUCCESS;
}


/**
 * Checks if an already optimized model is solvable.
 * @param model
 * @param solvable
 * @return SUCCESS/ERROR
 */
int optimized_model_is_solvable(GRBmodel* model, int *solvable) {
    int optimstatus;

    if (GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus)) {
        return ERROR;
    }

    if(optimstatus == GRB_OPTIMAL) {
        *solvable = TRUE;
    }
    else {
        *solvable = FALSE;
    }
    return SUCCESS;
}

/**
 * Gets a board and returns True/False.
 * @param board
 * @param solvable: pointer to return value.
 * @return ERROR/SUCCESS. Error means a Gurobi error.
 */
int is_solvable(sudoku_board_t *board, int *solvable) {
    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    var_list_t *var_list = NULL;

    if (get_enviroment(&env) != SUCCESS) {
        return ERROR;
    }

    if (init_model(env, &model) != SUCCESS) {
        return ERROR;
    }

    var_list = get_variables(board);

    if (run_gurobi_on_board(model, var_list, ILP) != SUCCESS) {
        GRBfreemodel(model);
        free_var_list(var_list);
        return ERROR;
    }

    if (optimized_model_is_solvable(model, solvable) != SUCCESS) {
        GRBfreemodel(model);
        free_var_list(var_list);
        return ERROR;
    }

    GRBfreemodel(model);
    free_var_list(var_list);
    return SUCCESS;
}

/**
 * Solves the board, rewriting on it.
 * @param board
 * @param type ILP/LP.
 * @return SUCCESS/ERROR.
 */
int solve_board(sudoku_board_t* board, enum programming_type type) {
    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    var_list_t *var_list = NULL;
    int solvable;

    if (get_enviroment(&env) != SUCCESS) {
        return ERROR;
    }

    if (init_model(env, &model) != SUCCESS) {
        return ERROR;
    }

    var_list = get_variables(board);

    if (run_gurobi_on_board(model, var_list, type) != SUCCESS) {
        return ERROR;
    }

    if (optimized_model_is_solvable(model, &solvable) != SUCCESS) {
        return ERROR;
    }

    if (!solvable)
        return UNSOLVABLE;

    /* TODO: LP */
    if (get_solved_board_ilp(model, var_list, board) != SUCCESS) {
        return ERROR;
    }

    return SUCCESS;
}


/**
 * Prints the last gurobi error.
 */
void print_gurobi_error() {
    GRBenv* env = NULL;
    if (!get_enviroment(&env)) {
        printf("Error: Gurobi: Could not get environment.");
    }
    printf("Error: Gurobi: %s\n", GRBgeterrormsg(env));
}

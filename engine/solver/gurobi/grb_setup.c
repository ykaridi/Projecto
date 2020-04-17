#include "grb_setup.h"

/** Gets the gurobi environment.
 * @param env_ptr
 * @return SUCCESS/ERROR value. env_ptr can change on error.
 */
int get_enviroment(GRBenv **env_ptr) {
    if (*env_ptr != NULL) {
        return SUCCESS;
    }

    if (GRBloadenv(env_ptr, LOG_FILE)) {
        return ERROR;
    }

    /* For debugging. */
    if (!GURUBI_OUTPUT) {
        if (GRBsetintparam(*env_ptr, "OutputFlag", 0)) {
            GRBfreeenv(*env_ptr);
            *env_ptr = NULL;
            return ERROR;
        }
    }
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
    double *obj = NULL;
    char *vtype = NULL;
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
    if (prog_type == LP) {
        obj = malloc(size * sizeof(double));
        if (!obj) {
            EXIT_ON_ERROR("malloc")
        }
        for (i = 0; i < size; ++i) {
            obj[i] = 1;
        }
    }
    vtype = malloc(size * sizeof(char));
    if (!vtype) {
        EXIT_ON_ERROR("malloc")
    }
    for (i = 0; i < size; ++i) {
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
 * Runs the ILP/LP Gurobi optimizer on the given board.
 * @param model
 * @param board
 * @param type
 * @return SUCCESS/ERROR.
 */
int run_gurobi_on_board(GRBmodel *model, var_list_t *var_list, enum programming_type type) {
    if (setup_model(model, var_list, type) != SUCCESS) {
        return ERROR;
    }

    if (GRBoptimize(model)) {
        return ERROR;
    }

    return SUCCESS;
}

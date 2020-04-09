#ifndef SUDOKU_GRB_H
#define SUDOKU_GRB_H

#include <stdbool.h>

#define LOG_FILE "grb.log"
#define DEFAULT_MODEL_NAME "sudoku_model"

#include "../../sudoku_board.h"
#include "gurobi_c.h"
#include "sudoku_var_list.h"


#define GURUBI_OUTPUT (0)

enum programming_type {
    ILP,
    LP
};


int get_enviroment(GRBenv **env_ptr);
int init_model(GRBenv *env, GRBmodel **model);
var_list_t *get_variables(sudoku_board_t *board);
int setup_model(GRBmodel *model, var_list_t *var_list, enum programming_type prog_type);

int run_gurobi_on_board(GRBmodel *model, var_list_t *var_list, enum programming_type type);
int optimized_model_is_solvable(GRBmodel *model, bool *solvable);

int get_solved_board(GRBmodel *model, const var_list_t *var_list, sudoku_board_t *board,
                     enum programming_type type, float threshold);

int get_single_cell_ilp(GRBmodel *model, const cell_var_list_t *var_list, int *value);
int get_single_cell_lp(GRBmodel *model, const cell_var_list_t *var_list, int *values, int* len);

#endif /*SUDOKU_GRB_H */

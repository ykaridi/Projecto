#ifndef GRB_SETUP_H
#define GRB_SETUP_H

#include "gurobi_c.h"
#include "grb_utils.h"
#include "../../../utils.h"
#include "../../sudoku_board.h"
#include "sudoku_var_list.h"


int get_enviroment(GRBenv **env_ptr);

int init_model(GRBenv *env, GRBmodel **model);

var_list_t *get_variables(sudoku_board_t *board);

int setup_model(GRBmodel *model, var_list_t *var_list, enum programming_type prog_type);

int run_gurobi_on_board(GRBmodel *model, var_list_t *var_list, enum programming_type type);


#endif /* GRB_SETUP_H */

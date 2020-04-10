#ifndef GRB_PROCESSING_H
#define GRB_PROCESSING_H

#include <stdbool.h>


#include "../../sudoku_board.h"
#include "gurobi_c.h"
#include "grb_utils.h"
#include "sudoku_var_list.h"

typedef union _single_cell_input_bare {
    int *ilp_value;

    struct {
        int *values;
        int *len;
    } lp_value;
} single_cell_input_bare_t;

typedef struct _single_cell_input {
    enum programming_type type;

    single_cell_input_bare_t input;
} single_cell_input_t ;


int optimized_model_is_solvable(GRBmodel *model, bool *solvable);

int get_solved_board(GRBmodel *model, const var_list_t *var_list, sudoku_board_t *board,
                     enum programming_type type, float threshold);


void create_single_cell_input_ilp(single_cell_input_t* single_cell_input, int* ilp_value);
void create_single_cell_input_lp(single_cell_input_t* single_cell_input, int* values, int* len);

int process_single_cell(GRBmodel *model, const cell_var_list_t *cell_var_list, single_cell_input_t* input);


#endif /*GRB_PROCESSING_H */

#ifndef SUDOKU_VAR_LIST_H
#define SUDOKU_VAR_LIST_H

typedef struct _variable{
    int taken;
    int index;
} variable_t;

typedef struct _cell_var_list {
    /* number of variables taken */
    int num_vars;
    /* Max length of vars. */
    int max_size;
    /* Variables array. */
    variable_t* vars;
} cell_var_list_t;

typedef struct _var_list {
    /* The whole board is size*size. */
    int rows;
    int cols;
    /* size = rows * cols */
    int size;
    cell_var_list_t*** cell_vars;
} var_list_t;

cell_var_list_t* init_cell_var_list(int size);
void free_cell_var_list(cell_var_list_t* list);

var_list_t* init_var_list(int rows, int cols);
void free_var_list(var_list_t* list);

void var_list_add_var(var_list_t* list, int i, int j, int k, int index);

void print_var_list(const var_list_t* list);

#endif /* SUDOKU_VAR_LIST_H */

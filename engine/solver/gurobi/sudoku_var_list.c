#include "sudoku_var_list.h"
#include "../../../utils.h"


/**
 * initialize a cell_var_list.
 * @return
 */
cell_var_list_t *init_cell_var_list(int size) {
    cell_var_list_t *cell_var_list = malloc(sizeof(cell_var_list_t));
    if (!cell_var_list) {
        EXIT_ON_ERROR("malloc")
    }
    cell_var_list->max_size = size;
    cell_var_list->num_vars = 0;
    cell_var_list->vars = calloc(sizeof(variable_t), size);
    return cell_var_list;
}

/**
 * frees list.
 * @param list
 */
void free_cell_var_list(cell_var_list_t *list) {
    free(list->vars);
    free(list);
}

/**
 * vars.append({k, index})
 * @param k
 * @param index
 * @return
 */
void cell_list_add_var(cell_var_list_t *list, int k, int index) {
    list->vars[k].taken = TRUE;
    list->vars[k].index = index;
    list->num_vars++;
}


/**
 * initialize the var list.
 */
var_list_t *init_var_list(int rows, int cols) {
    int i, j, size = rows * cols;

    var_list_t *var_list = malloc(sizeof(var_list_t));
    if (!var_list) {
        EXIT_ON_ERROR("malloc")
    }
    var_list->rows = rows;
    var_list->cols = cols;
    var_list->size = size;
    var_list->cell_vars = malloc(size * sizeof(cell_var_list_t *));
    if (!var_list->cell_vars) {
        EXIT_ON_ERROR("malloc")
    }
    for (i = 0; i < size; ++i) {
        var_list->cell_vars[i] = malloc(size * sizeof(cell_var_list_t));
        if (!var_list->cell_vars[i]) {
            EXIT_ON_ERROR("malloc")
        }
        for (j = 0; j < size; ++j) {
            var_list->cell_vars[i][j] = init_cell_var_list(size);
        }
    }
    return var_list;
}


/**
 * frees list.
 * @param list
 */
void free_var_list(var_list_t *list) {
    int i, j, size = list->size;
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            free_cell_var_list(list->cell_vars[i][j]);
        }
        free(list->cell_vars[i]);
    }
    free(list->cell_vars);
    free(list);
}

/**
 * list[i][j].append(k)
 * @param list
 * @param i
 * @param j
 * @param k
 * @return ERROR if
 */
void var_list_add_var(var_list_t *list, int i, int j, int k, int index) {
    cell_list_add_var(list->cell_vars[i][j], k, index);
}

void print_cell_var_list(cell_var_list_t *list) {
    int i = 0;
    printf("len=%d: [", list->num_vars);
    for (i = 0; i < list->max_size; ++i) {
        printf(" (%d, %d) ", list->vars[i].taken, list->vars[i].index);
    }
    printf("]");
}

void print_var_list(const var_list_t *list) {
    int i, j, size = list->size;
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            print_cell_var_list(list->cell_vars[i][j]);
            printf("\t");
        }
        printf("\n");
    }
}

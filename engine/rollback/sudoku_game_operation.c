#include "sudoku_game_operation.h"

sudoku_game_operation_t *create_atomic_game_operation() {
    sudoku_game_operation_t *operation = malloc(sizeof(sudoku_game_operation_t));
    if (operation == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    operation->operation_type = ATOMIC;
    return operation;
}

sudoku_game_operation_t *create_head_game_operation() {
    sudoku_game_operation_t *operation = malloc(sizeof(sudoku_game_operation_t));
    if (operation == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    operation->operation_type = HEAD;
    return operation;
}

sudoku_game_operation_t *create_composite_game_operation(enum sudoku_game_operation_type operation_type, int arg1, int arg2, int arg3, float farg) {
    sudoku_game_operation_t *operation = malloc(sizeof(sudoku_game_operation_t));
    operation_node_t *operation_list = create_operation_node();
    if (operation == NULL || operation_list == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    operation->operation_type = operation_type;
    operation->value.composite_operation.head = operation_list;
    operation->value.composite_operation.arg1 = arg1;
    operation->value.composite_operation.arg2 = arg2;
    operation->value.composite_operation.arg3 = arg3;
    operation->value.composite_operation.farg = farg;

    return operation;
}
void destruct_game_operation(sudoku_game_operation_t *operation) {
    if (operation->operation_type != ATOMIC && operation->operation_type != HEAD) {
        destruct_operation_list(operation->value.composite_operation.head);
    }

    free(operation);
}

void append_atomic_operation(sudoku_game_operation_t *composite_operation, int row, int col, int old_value, int new_value) {
    sudoku_game_operation_t *operation = create_atomic_game_operation();
    operation->value.atomic_operation.row = row;
    operation->value.atomic_operation.col = col;
    operation->value.atomic_operation.old_value = old_value;
    operation->value.atomic_operation.new_value = new_value;

    operation_list_append(composite_operation->value.composite_operation.head, operation);
}

void undo_operation(sudoku_game_t *game, sudoku_game_operation_t *operation) {
    operation_node_t *node = operation->value.composite_operation.head;

    while (node != NULL) {
        set_cell_flattened(game->board, node->operation->value.atomic_operation.old_value,
                           node->operation->value.atomic_operation.row, node->operation->value.atomic_operation.col);
        node = node->next;
    }
}
void redo_operation(sudoku_game_t *game, sudoku_game_operation_t *operation) {
    operation_node_t *node = operation->value.composite_operation.head;

    while (node != NULL) {
        set_cell_flattened(game->board, node->operation->value.atomic_operation.new_value,
                           node->operation->value.atomic_operation.row, node->operation->value.atomic_operation.col);
        node = node->next;
    }
}

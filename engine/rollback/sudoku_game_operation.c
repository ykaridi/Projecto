#include "sudoku_game_operation.h"

/**
 * Creates an empty atomic game operation
 * @return Pointer to the game operation object
 */
sudoku_game_operation_t *create_atomic_game_operation() {
    sudoku_game_operation_t *operation = malloc(sizeof(sudoku_game_operation_t));
    /* Check if malloc was successful */
    if (operation == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    operation->operation_type = ATOMIC;
    return operation;
}

/**
 * Creates a HEAD game operation, used to mark the beginning of an operation list
 * @return
 */
sudoku_game_operation_t *create_head_game_operation() {
    sudoku_game_operation_t *operation = malloc(sizeof(sudoku_game_operation_t));
    /* Check if malloc was successful */
    if (operation == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    operation->operation_type = HEAD;
    return operation;
}

/**
 * Create a composite game operation, representing a command given by the user
 * @param operation_type
 * @param arg1 Optional value, integer argument #1
 * @param arg2 Optional value, integer argument #2
 * @param arg3 Optional value, integer argument #3
 * @param farg Optional value, floating point argument
 * @return
 */
sudoku_game_operation_t *create_composite_game_operation(enum sudoku_game_operation_type operation_type, int arg1, int arg2, int arg3, float farg) {
    sudoku_game_operation_t *operation = malloc(sizeof(sudoku_game_operation_t));
    operation_node_t *operation_list = create_operation_node();
    sudoku_game_operation_t *head = create_head_game_operation();

    operation_list_append(operation_list, head);

    operation->operation_type = operation_type;
    operation->value.composite_operation.head = operation_list;
    operation->value.composite_operation.arg1 = arg1;
    operation->value.composite_operation.arg2 = arg2;
    operation->value.composite_operation.arg3 = arg3;
    operation->value.composite_operation.farg = farg;

    return operation;
}

/**
 * Destroy (free) a sudoku game operation
 * @param operation
 */
void destruct_game_operation(sudoku_game_operation_t *operation) {
    if (operation->operation_type != ATOMIC && operation->operation_type != HEAD) {
        /* Destruct atomic operations comprising the composite operation */
        destruct_operation_list(operation->value.composite_operation.head);
    }

    free(operation);
}

/**
 * Append an atomic game operation to a composite operation
 * @param composite_operation
 * @param row
 * @param col
 * @param old_value
 * @param new_value
 */
void append_atomic_operation(sudoku_game_operation_t *composite_operation, int row, int col, int old_value, int new_value) {
    sudoku_game_operation_t *operation = create_atomic_game_operation();
    operation->value.atomic_operation.row = row;
    operation->value.atomic_operation.col = col;
    operation->value.atomic_operation.old_value = old_value;
    operation->value.atomic_operation.new_value = new_value;

    operation_list_append(composite_operation->value.composite_operation.head, operation);
}

/**
 * Undo a composite game operation
 * @param game
 * @param operation
 */
void undo_operation(sudoku_game_t *game, sudoku_game_operation_t *operation) {
    operation_node_t *node = operation->value.composite_operation.head->next;

    /* Iterate over all atomic operations and undo them */
    while (node != NULL) {
        set_cell_flattened(game->board, node->operation->value.atomic_operation.old_value,
                           node->operation->value.atomic_operation.row, node->operation->value.atomic_operation.col);
        node = node->next;
    }
}

/**
 * Redo a composite game operation
 * @param game
 * @param operation
 */
void redo_operation(sudoku_game_t *game, sudoku_game_operation_t *operation) {
    operation_node_t *node = operation->value.composite_operation.head->next;

    /* Iterate over all atomic operations and redo them */
    while (node != NULL) {
        set_cell_flattened(game->board, node->operation->value.atomic_operation.new_value,
                           node->operation->value.atomic_operation.row, node->operation->value.atomic_operation.col);
        node = node->next;
    }
}

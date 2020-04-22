#ifndef OPERATION_LIST_H
#define OPERATION_LIST_H

typedef struct _operation_node operation_node_t;

#include "sudoku_game_operation.h"
#include "../../utils.h"

/**
 * A struct for a doubly linked operation list (for undo/redo)
 */
struct _operation_node {
    sudoku_game_operation_t *operation;
    struct _operation_node *prev;
    struct _operation_node *next;
};

operation_node_t *create_operation_node();
void destruct_operation_list(operation_node_t *head);

void operation_list_delete(operation_node_t *node);
void operation_list_delete_after(operation_node_t *node);
int operation_list_append(operation_node_t *node, sudoku_game_operation_t *operation);

#endif

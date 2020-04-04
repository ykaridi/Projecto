#ifndef OPERATION_LIST_H
#define OPERATION_LIST_H

#include "../../utils.h"
#include "../sudoku_game_operation.h"

typedef struct _operation_node {
    sudoku_game_operation_t *operation;
    struct _operation_node *prev;
    struct _operation_node *next;
} operation_node_t;

operation_node_t *create_operation_list();
void destruct_operation_list(operation_node_t *head);

void operation_list_delete(operation_node_t *node);
void operation_list_clear_after(operation_node_t *node);
void operation_list_append(operation_node_t *node, sudoku_game_operation_t *operation);

#endif

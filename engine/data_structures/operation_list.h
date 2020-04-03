#ifndef OPERATION_LIST_H
#define OPERATION_LIST_H

#include "../sudoku_game_operation.h"

typedef struct _operation_node {
    sudoku_game_operation_t *operation;
    struct _operation_node *prev;
    struct _operation_node *next;
} operation_node_t;

void operation_list_delete(operation_node_t *node);
void operation_list_clear_after(operation_node_t *node);
void operation_list_append(operation_node_t *node, sudoku_game_operation_t *operation);

#endif //OPERATION_LIST_H

#include "operation_list.h"

operation_node_t *create_operation_list() {
    operation_node_t *head = malloc(sizeof(operation_node_t));
    if (head == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    head->operation = NULL;
    head->prev = NULL;
    head -> next = NULL;

    return head;
}

void destruct_operation_list(operation_node_t *node) {
    while (node->prev != NULL) {
        node = node->prev;
    }

    operation_list_delete(node);
}

void operation_list_delete(operation_node_t *node) {
    /* TODO: Implement */
}
void operation_list_clear_after(operation_node_t *node) {
    /* TODO: Implement */
}
void operation_list_append(operation_node_t *node, sudoku_game_operation_t *operation) {
    /* TODO: Implement */
}
#include "operation_list.h"

/**
 * Create a doubly linked game operation node object
 * @return A pointer to the node object
 */
operation_node_t *create_operation_node() {
    operation_node_t *head = malloc(sizeof(operation_node_t));
    /* Check if malloc was successful */
    if (head == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    head->operation = NULL;
    head->prev = NULL;
    head->next = NULL;

    return head;
}

/**
 * Destroy (free) a doubly linked game operation list
 * @param node
 */
void destruct_operation_list(operation_node_t *node) {
    /* Find first node in the list */
    while (node->prev != NULL) {
        node = node->prev;
    }

    operation_list_delete(node);
}

/**
 * Remove (and destroy) the current node and all following nodes
 * @param node
 */
void operation_list_delete(operation_node_t *node) {
    operation_node_t *next;
    if (node->prev != NULL)
        node->prev->next = NULL;

    while (node != NULL) {
        next = node->next;
        if (node->operation != NULL)
            destruct_game_operation(node->operation);
        free(node);
        node = next;
    }
}

/**
 * Remove (and destroy) all following nodes
 * @param node
 */
void operation_list_delete_after(operation_node_t *node) {
    if (node->next != NULL)
        operation_list_delete(node->next);

    node->next = NULL;
}

/**
 * Append a game operation to a game operation doubly linked list
 * @param node
 * @param operation
 * @return number of new nodes in the list
 */
int operation_list_append(operation_node_t *node, sudoku_game_operation_t *operation) {
    if (node->operation == NULL) {
        node->operation = operation;
        return 0;
    }

    while (node->next != NULL)
        node = node->next;

    node->next = create_operation_node();
    node->next->prev = node;
    node->next->operation = operation;
    return 1;
}

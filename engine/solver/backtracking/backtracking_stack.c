#include <malloc.h>
#include "backtracking_stack.h"
#include "../../../utils.h"

/**
 * Create a backtracking stack object
 * @return
 */
backtracking_stack_t* create_backtracking_stack() {
    backtracking_stack_t *stack = malloc(sizeof(backtracking_stack_t));
    if (stack == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    stack->head = NULL;
    return stack;
}

/**
 * Destroy (free) a backtracking stack object
 * @param stack
 */
void destroy_backtracking_stack(backtracking_stack_t *stack) {
    backtracking_stack_node_t *current, *tmp;

    if (!backtracking_stack_empty(stack)) {
        current = stack->head;
        while (current != NULL) {
            tmp = current;
            current = current->prev;
            free(tmp);
        }
    }

    free(stack);
}

/**
 * Checks if a backtracking stack is empty
 * @param stack
 * @return
 */
int backtracking_stack_empty(backtracking_stack_t *stack) {
    return stack->head == NULL;
}

/**
 * Peek at the top value of a backtracking stack
 * @param stack
 * @return
 */
int backtracking_stack_peek(backtracking_stack_t *stack) {
    return !backtracking_stack_empty(stack) ? stack->head->idx : -1;
}

/**
 * Pop the top object from a backtracking stack
 * @param stack
 * @return
 */
int backtracking_stack_pop(backtracking_stack_t *stack) {
    int value = backtracking_stack_peek(stack);
    backtracking_stack_node_t *prev;

    if (!backtracking_stack_empty(stack)) {
        prev = stack->head->prev;
        free(stack->head);
        stack->head = prev;
    }

    return value;
}

/**
 * Push an object to the top of a backtracking stack
 * @param stack
 * @param idx
 */
void backtracking_stack_push(backtracking_stack_t *stack, int idx) {
    backtracking_stack_node_t *node;

    node = malloc(sizeof(backtracking_stack_node_t));
    if (node == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    node->idx = idx;
    node->prev = stack->head;
    stack->head = node;
}

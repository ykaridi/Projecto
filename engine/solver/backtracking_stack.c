#include <malloc.h>
#include "backtracking_stack.h"
#include "../../utils.h"

backtracking_stack_t* create_backtracking_stack() {
    backtracking_stack_t *stack = malloc(sizeof(backtracking_stack_t));
    if (stack == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    stack->head = NULL;
    return stack;
}
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

int backtracking_stack_empty(backtracking_stack_t *stack) {
    return stack->head == NULL;
}
int backtracking_stack_peek(backtracking_stack_t *stack) {
    return !backtracking_stack_empty(stack) ? stack->head->idx : -1;
}
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

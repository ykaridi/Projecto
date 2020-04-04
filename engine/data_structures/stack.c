#include <malloc.h>
#include "stack.h"
#include "../../utils.h"

stack_t* create_stack() {
    stack_t *stack = malloc(sizeof(stack_t));
    if (stack == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    stack->head = NULL;
    return stack;
}
void destroy_stack(stack_t *stack) {
    stack_node_t *current, *tmp;

    if (!stack_empty(stack)) {
        current = stack->head;
        while (current != NULL) {
            tmp = current;
            current = current->next;
            free(tmp);
        }
    }

    free(stack);
}

int stack_empty(stack_t *stack) {
    return stack->head == NULL;
}
int stack_peek(stack_t *stack) {
    stack_node_t *current;

    if (!stack_empty(stack)) {
        current = stack->head;
        while (current->next != NULL)
            current = current-> next;

        return current->value;
    } else {
        return 0;
    }
}
int stack_pop(stack_t *stack) {
    int value;
    stack_node_t *current;

    if (!stack_empty(stack)) {
        current = stack->head;
        if (current->next == NULL) {
            stack->head = NULL;
            free(current);
            return current->value;
        }

        while (current->next->next != NULL)
            current = current->next;

        value = current->next->value;
        free(current->next);
        current->next = NULL;
        return value;
    } else
        return 0;
}
void stack_push(stack_t *stack, int value) {
    stack_node_t *node, *current;

    node = malloc(sizeof(stack_node_t));
    if (node == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    node->value = value;
    node->next = NULL;

    if (stack_empty(stack)) {
        stack->head = node;
    } else {
        current = stack->head;
        while (current->next != NULL)
            current = current->next;

        current->next = node;
    }
}

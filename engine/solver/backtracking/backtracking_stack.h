#ifndef STACK_H
#define STACK_H

/**
 * A struct representing a single item in the backtracking stack simulation
 */
typedef struct _backtracking_stack_node {
     int idx;
     struct _backtracking_stack_node *prev;
} backtracking_stack_node_t;

/**
 * A struct representing the backtracking stack (for recursion simulation)
 */
typedef struct _backtracking_stack {
    backtracking_stack_node_t *head;
} backtracking_stack_t;

backtracking_stack_t* create_backtracking_stack();
void destroy_backtracking_stack(backtracking_stack_t *stack);

int backtracking_stack_empty(backtracking_stack_t *stack);
int backtracking_stack_peek(backtracking_stack_t *stack);
int backtracking_stack_pop(backtracking_stack_t *stack);
void backtracking_stack_push(backtracking_stack_t *stack, int idx);

#endif

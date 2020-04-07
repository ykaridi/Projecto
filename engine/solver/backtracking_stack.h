#ifndef STACK_H
#define STACK_H

typedef struct _backtracking_stack_node {
     int idx;
     struct _backtracking_stack_node *prev;
} backtracking_stack_node_t;

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

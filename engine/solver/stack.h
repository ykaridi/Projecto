#ifndef STACK_H
#define STACK_H

typedef struct _stack_node {
     int value;
     struct _stack_node *next;
} stack_node_t;

typedef struct _stack {
    stack_node_t *head;
} stack_t;

stack_t* create_stack();
void destroy_stack(stack_t *stack);

int stack_empty(stack_t *stack);
int stack_peek(stack_t *stack);
int stack_pop(stack_t *stack);
void stack_push(stack_t *stack, int value);

#endif

// stack.h
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdlib.h>

#define STACK_EMPTY INT_MIN

typedef struct StackNode {
  int value;
  struct StackNode* next;
} StackNode;

typedef StackNode* Stack;

bool isEmpty(Stack* stack);
void push(Stack* stack, int value);
int pop(Stack* stack);

#endif // STACK_H
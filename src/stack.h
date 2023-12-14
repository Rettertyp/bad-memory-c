// stack.h
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdlib.h>

#define STACK_EMPTY INT_MIN

/**
 * @brief Represents a node in a stack.
 *
 * This struct contains the value of the node and a pointer to the next node in
 * the stack.
 */
typedef struct StackNode {
  int value;              /** The value of the node. */
  struct StackNode* next; /** Pointer to the next node in the stack. */
} StackNode;

/**
 * @brief Definition of the Stack data structure.
 *
 * The Stack type represents a stack data structure.
 * It is implemented as a pointer to a StackNode.
 */
typedef StackNode* Stack;

void push(Stack* stack, int value);
int pop(Stack* stack);
bool stackIsEmpty(Stack* stack);
bool stackEquals(Stack* stack1, Stack* stack2);
Stack copyStack(Stack stack);

#endif // STACK_H
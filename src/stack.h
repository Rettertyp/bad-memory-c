#ifndef STACK_H
#define STACK_H

#include "graphNode.h"
#include <stdbool.h>

typedef struct GraphNode GraphNode;

/**
 * @brief Represents a node in a stack.
 *
 * This struct intervalContains the value of the node and a pointer to the next node in
 * the stack.
 */
typedef struct StackNode {
  GraphNode* value;       /** The value of the node. */
  struct StackNode* next; /** Pointer to the next node in the stack. */
} StackNode;

/**
 * @brief Definition of the Stack data structure.
 *
 * The Stack type represents a stack data structure.
 * It is implemented as a pointer to a StackNode.
 */
typedef StackNode* Stack;

void stackPush(Stack* stack, GraphNode* value);
GraphNode* stackPop(Stack* stack);
GraphNode* stackTop(const Stack* stack);
void stackDelete(Stack* stack);
bool stackIsEmpty(const Stack* stack);
bool stackEquals(const Stack stack1, const Stack stack2);
void stackPrint(const Stack* stack);
Stack stackCopy(const Stack stack);

#endif // STACK_H
#include "stack.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Pushes a new element onto the stack.
 *
 * @param stack The stack to push the element onto.
 * @param value The value to be pushed onto the stack.
 */
void push(Stack* stack, GraphNode* value) {
  StackNode* newNode = malloc(sizeof(StackNode));

  newNode->value = value;
  newNode->next = *stack;

  // update the head of the stack
  *stack = newNode;
}

/**
 * Pops the top element from the stack.
 *
 * @param stack The stack to pop the element from.
 * @return The popped element, or STACK_EMPTY if the stack is empty.
 */
GraphNode* pop(Stack* stack) {
  if (*stack == NULL)
    return NULL;

  GraphNode* result = (*stack)->value;

  // save top node to variable, to be able to free it afterwards
  StackNode* topNode = *stack;
  *stack = (*stack)->next;
  free(topNode);

  return result;
}

/**
 * Returns the top element of the stack.
 *
 * @param stack The stack to retrieve the top element from.
 * @return The top element of the stack, or NULL if the stack is empty.
 */
GraphNode* top(const Stack* stack) {
  if (*stack == NULL)
    return NULL;

  return (*stack)->value;
}

/**
 * Checks if the stack is empty.
 *
 * @param stack The stack to be checked.
 * @return True if the stack is empty, false otherwise.
 */
bool stackIsEmpty(const Stack* stack) { return *stack == NULL; }

/**
 * Compares the values of each node in the two stacks to determine if
 * they are equal. The stacks are considered equal if they have the
 * same number of nodes and each corresponding node has the same value.
 *
 * @param stack1 Pointer to the first stack.
 * @param stack2 Pointer to the second stack.
 * @return true if the stacks are equal, false otherwise.
 */
bool stackEquals(const Stack stack1, const Stack stack2) {
  StackNode* node1 = stack1;
  StackNode* node2 = stack2;

  while (node1 != NULL && node2 != NULL) {
    if (node1->value != node2->value)
      return false;

    node1 = node1->next;
    node2 = node2->next;
  }

  return node1 == NULL && node2 == NULL;
}

/**
 * Prints the elements of the stack.
 *
 * @param stack The stack to be printed.
 */
void printStack(const Stack* stack) {
  StackNode* currNode = *stack;

  debug_print("\n[\n");

  while (currNode) {
    printGraphNode(currNode->value);

    currNode = currNode->next;
  }

  debug_print("]\n");
}

/**
 * Recursively pushes the values of a linked list backwards onto a stack.
 *
 * @param stack The stack to push the values onto.
 * @param node The current node in the linked list.
 */
static void pushBackwards(Stack* stack, StackNode* node) {
  if (!node) {
    return;
  }

  pushBackwards(stack, node->next);
  push(stack, node->value);
}

/**
 * Creates a copy of the given stack.
 *
 * @param stack The stack to be copied.
 * @return A new stack that is a copy of the original stack.
 */
Stack copyStack(const Stack stack) {
  Stack copy = NULL;

  pushBackwards(&copy, stack);

  return copy;
}
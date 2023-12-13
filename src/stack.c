#include "stack.h"

/**
 * Pushes a value onto the stack.
 *
 * @param stack The stack to push the value onto.
 * @param value The value to be pushed onto the stack.
 */
void push(Stack* stack, int value) {
  StackNode* newNode = malloc(sizeof(StackNode));

  newNode->value = value;
  newNode->next = *stack;

  // update the head of the stack
  *stack = newNode;
}

/**
 * Pops the top value from the stack.
 *
 * @param stack The stack to pop the value from.
 * @return The popped value, or STACK_EMPTY if the stack is empty.
 */
int pop(Stack* stack) {
  if (*stack == NULL)
    return STACK_EMPTY;

  int result = (*stack)->value;

  // save top node to variable, to be able to free it afterwards
  StackNode* topNode = *stack;
  *stack = (*stack)->next;
  free(topNode);

  return result;
}

/**
 * Checks if the stack is empty.
 *
 * @param stack The stack to be checked.
 * @return True if the stack is empty, false otherwise.
 */
bool isEmpty(Stack* stack) { return *stack == NULL; }

/**
 * Compares the values of each node in the two stacks to determine if
 * they are equal. The stacks are considered equal if they have the
 * same number of nodes and each corresponding node has the same value.
 *
 * @param stack1 Pointer to the first stack.
 * @param stack2 Pointer to the second stack.
 * @return true if the stacks are equal, false otherwise.
 */
bool equals(Stack* stack1, Stack* stack2) {
  StackNode* node1 = *stack1;
  StackNode* node2 = *stack2;

  while (node1 != NULL && node2 != NULL) {
    if (node1->value != node2->value)
      return false;

    node1 = node1->next;
    node2 = node2->next;
  }

  return node1 == NULL && node2 == NULL;
}
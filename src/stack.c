#include "stack.h"

/**
 * Checks if the stack is empty.
 *
 * @param stack The stack to be checked.
 * @return True if the stack is empty, false otherwise.
 */
bool isEmpty(Stack* stack) { return *stack == NULL; }

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
#include "stack.h"
#include <stdio.h>

int main(int argc, char** argv) {
  Stack s1 = NULL;
  push(&s1, 1);
  push(&s1, 2);
  push(&s1, 3);
  push(&s1, 4);

  if (!isEmpty(&s1))
    printf("Stack is not empty\n");

  int val;
  while ((val = pop(&s1)) != STACK_EMPTY)
    printf("%d\n", val);

  if (isEmpty(&s1))
    printf("Stack is empty\n");

  return 0;
}
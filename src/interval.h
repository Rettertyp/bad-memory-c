#ifndef INTERVAL_H
#define INTERVAL_H

#include <stdbool.h>

typedef struct Interval {
  int bottom;
  int top;
} Interval;

Interval* createInterval(int bottom, int top);

#endif // INTERVAL_H
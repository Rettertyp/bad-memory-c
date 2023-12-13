#ifndef INTERVAL_H
#define INTERVAL_H

#include <stdbool.h>

/**
 * @brief Represents an interval of integers with a bottom and top value.
 */
typedef struct Interval {
  int bottom; /** The bottom value of the interval. */
  int top;    /** The top value of the interval. */
} Interval;

Interval* createInterval(int bottom, int top);
bool contains(Interval* interval, int value);
bool greaterThan(Interval* interval, int value);
bool lessThan(Interval* interval, int value);

#endif // INTERVAL_H
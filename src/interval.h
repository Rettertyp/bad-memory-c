#ifndef INTERVAL_H
#define INTERVAL_H

#include <stdbool.h>

/**
 * @brief Represents an interval of integers with a bottom and top value.
 */
typedef struct Interval {
  unsigned int bottom; /** The bottom value of the interval. */
  unsigned int top;    /** The top value of the interval. */
} Interval;

Interval* intervalCreate(const unsigned int bottom, const unsigned int top);
bool intervalContains(const Interval* interval, const unsigned int value);
bool intervalGreaterThan(const Interval* interval, const unsigned int value);
bool intervalGreaterEqual(const Interval* interval, const unsigned int value);
bool intervalLessThan(const Interval* interval, const unsigned int value);

#endif // INTERVAL_H
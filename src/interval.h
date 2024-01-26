#ifndef INTERVAL_H
#define INTERVAL_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Represents an interval of integers with a bottom and top value.
 */
typedef struct Interval {
  uint32_t bottom; /** The bottom value of the interval. */
  uint32_t top;    /** The top value of the interval. */
  uint32_t amount; /** The amount of intervals with those properties. */
} Interval;

Interval* intervalCreate(const uint32_t bottom, const uint32_t top, const uint32_t amount);
bool intervalContains(const Interval* interval, const uint32_t value);
bool intervalGreaterThan(const Interval* interval, const uint32_t value);
bool intervalGreaterEqual(const Interval* interval, const uint32_t value);
bool intervalLessThan(const Interval* interval, const uint32_t value);

#endif // INTERVAL_H
#ifndef INSTANCEGEN_H
#define INSTANCEGEN_H

#include "intervalSet.h"
#include <stdint.h>

typedef struct InstanceInfo {
  IntervalSet* intervalSet;
  uint32_t metadataLength;
  uint32_t* metadata;
} InstanceInfo;

void instanceInitRandom();
InstanceInfo instanceSimpleYes(const uint32_t n);
InstanceInfo instanceSimpleNo(const uint32_t n);
InstanceInfo instanceMaxWhitnessesYes(const uint32_t n);
InstanceInfo instanceMaxWhitnessesNo(const uint32_t n);
InstanceInfo instanceMaxGroupWhitnessesYes(const uint32_t n);
InstanceInfo instanceMaxGroupWhitnessesNo(const uint32_t n);
InstanceInfo instanceHardYesAmountVersion(const uint32_t n);
InstanceInfo instanceHardNoAmountVersion(const uint32_t n);
InstanceInfo instanceAllFull(const uint32_t n);

#endif // INSTANCEGEN_H
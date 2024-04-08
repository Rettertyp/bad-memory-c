#ifndef INSTANCEGEN_H
#define INSTANCEGEN_H

#include "intervalSet.h"
#include <stdint.h>

#define INSTANCE_NAME_LENGTH 100

typedef struct InstanceInfo {
  IntervalSet* intervalSet;
  char name[INSTANCE_NAME_LENGTH];
  uint32_t metadataLength;
  uint32_t* metadata;
} InstanceInfo;

void instanceInitRandom();
InstanceInfo instanceSimpleYes(const uint32_t n);
InstanceInfo instanceSimpleNo(const uint32_t n);
InstanceInfo instanceMaxWitnessesYes(const uint32_t n);
InstanceInfo instanceMaxWitnessesNo(const uint32_t n);
InstanceInfo instanceMaxGroupWitnessesYes(const uint32_t n);
InstanceInfo instanceMaxGroupWitnessesNo(const uint32_t n);
InstanceInfo instanceHardYesAmountVersion(const uint32_t n);
InstanceInfo instanceHardNoAmountVersion(const uint32_t n);
InstanceInfo instanceAllFull(const uint32_t n);

#endif // INSTANCEGEN_H
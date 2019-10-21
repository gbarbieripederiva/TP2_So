#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <stdint.h>

void initializeMemory();
void * giveMeMemory(uint64_t size);
int unGiveMeMemory(void * location);





#endif

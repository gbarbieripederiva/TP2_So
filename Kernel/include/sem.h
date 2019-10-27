#ifndef SEM_H
#define SEM_H

# include <semaphore.h>
#include <stdint.h>

void enter_region(uint64_t lock,semaphore sem, int pid);
void leave_region(uint64_t lock, semaphore sem);

#endif
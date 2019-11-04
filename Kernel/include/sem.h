#ifndef SEM_H
#define SEM_H

#include <semaphores.h>
#include <stdint.h>

void enter_region(uint64_t lock,semaphore semaph, int pid);
void leave_region(uint64_t lock, semaphore semaph);
void xchg1(uint64_t lock);

#endif
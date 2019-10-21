#ifndef SCHEDULER_H
#define SCHEDULER_H 
#include <stdint.h>
#include <process.h>



struct procInSched{
    processInfo process;
    int entered;
    int state;
};
typedef struct procInSched procInSched;

void init_sched();
int run_process(processInfo process, int state);
void init_iterator();
int kill_process(int pid);
uint64_t scheduler(uint64_t sp);
int set_state(int pid, int state);








#endif
#ifndef SCHEDULER_H
#define SCHEDULER_H 
#include <stdint.h>
#include <process.h>
#define HALT 3
#define EMPTY 2
#define BLOCKED 0
#define READY 1
#define SIZE 10




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
int get_current_pid();
void print_running_procs();
processInfo getCurrentProcess();
void setStdin(int pid,int fd);// set process stdin
void setStdout(int pid,int fd);// set process stdout
#endif
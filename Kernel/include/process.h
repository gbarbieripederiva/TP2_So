#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>
#define MAX_PROCESSES 300
#define STACK_SIZE 8192


typedef struct process{
    int ppid;
    int pid;
    int priority;
    uint64_t stack_end;
    uint64_t sp;
    int stdin;
    int stdout;
} process;

typedef struct process * processInfo;

void init_processes();

processInfo create_process(int priority, uint64_t rip);
processInfo create_process_with_args(int priority, uint64_t rip, uint64_t arg1, uint64_t arg2);

int set_priority(int pid, int priority);

#endif
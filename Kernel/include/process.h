#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>
#define MAX_PROCESSES 300
#define STACK_SIZE 8192

typedef struct process{
    int ppid;
    int pid;
    char * name;
    int priority;
    uint64_t stack_end;
    uint64_t sp;
} process;

typedef struct process * processInfo;

void init_processes();

processInfo create_process(char * name, int priority, uint64_t rip);

int set_priority(int pid, int priority);

#endif
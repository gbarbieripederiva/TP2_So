#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>
#define MAX_PROCESSES 300
#define STACK_SIZE 8192
#define STDIN 0
#define STDOUT 1


typedef struct process{
    int ppid;
    int pid;
    int priority;
    int fd[2];
    uint64_t stack_end;
    uint64_t sp;
} process;

typedef struct process * processInfo;

void init_processes();

processInfo create_process(int priority, uint64_t rip);
processInfo create_process_with_args(int priority, uint64_t rip, uint64_t arg1, uint64_t arg2);

int set_priority(int pid, int priority);
int set_std_in(int pid, int fd);
int set_std_out(int pid, int fd);

#endif
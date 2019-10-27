#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include <stdint.h>
#include <memoryManager.h>
#include <sem.h>
#include <scheduler.h>
#include <null.h>


typedef struct process_node * node_pointer;
struct process_node{
    int pid;
    node_pointer next;
};


typedef struct sem{
    char *name;
    int state;
    int sem_id;
    node_pointer waiting_proc;
} sem;

typedef sem * semaphore;

void init_sems();
semaphore create_sem_struct(char * name, int state);
int s_open(char * name, int state);
int s_close(int sid);
void add_waiting_proc(uint64_t lock, semaphore sem, int pid);
int s_wait(int sid);
void check_blocked(uint64_t lock, semaphore sem);
int sem_post(int sid);



#endif
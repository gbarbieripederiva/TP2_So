#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#define SEM_LOCKED 1
#define SEM_UNLOCKED 0

typedef struct process_node * node_pointer;
struct process_node{
    int pid;
    node_pointer next;
};


typedef struct sem{
    int name;
    int state;
    int sem_id;
    node_pointer waiting_proc;
} sem;

typedef sem *semaphore;

#include <stdint.h>
#include <memoryManager.h>
#include <sem.h>
#include <scheduler.h>
#include <null.h>
#include <naiveConsole.h>
#include <interrupts.h>
#include <lib.h>




void init_sems();
semaphore create_sem_struct(int name, int state);
int s_open(int name, int state);
int s_close(int sid);
void add_waiting_proc(uint64_t lock, semaphore sem, int pid);
int s_wait(int sid);
void check_blocked(uint64_t lock, semaphore sem);
int s_post(int sid);
void print_sems();



#endif
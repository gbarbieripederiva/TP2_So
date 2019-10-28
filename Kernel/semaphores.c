
#include <semaphores.h>

#define MAX_SEMS 10





int sem_id;
semaphore semaphores[MAX_SEMS];

void init_sems(){
    int i=0;
    while(i < MAX_SEMS){
        semaphores[i++] = NULL;
    }
    sem_id = 0;
}

semaphore create_sem_struct(char * name, int state){
    semaphore aux1 = (semaphore) giveMeMemory (sizeof(sem));
    aux1 -> name = name;
    aux1 -> sem_id = sem_id++;
    aux1 -> state = state;
    aux1 -> waiting_proc = NULL;
    return aux1;
}


int s_open(char * name, int state){
    int i = 0;
    while(i < MAX_SEMS && semaphores[i] != NULL){
        i++;
    }
    if(i == MAX_SEMS){
        return -1;
    }
    else{
        semaphores[i] = create_sem_struct(name, state);
        return semaphores[i] -> sem_id;
    }
}

void free_waiting_procs(semaphore sem){
    while(sem ->waiting_proc != NULL){
        node_pointer aux = sem -> waiting_proc;
        sem ->waiting_proc = sem ->waiting_proc ->next;
        unGiveMeMemory((void *)aux);
    }
}

int s_close(int sid){
    int i = 0;
    while(i < MAX_SEMS && (semaphores[i] == NULL || semaphores[i] -> sem_id != sid)){
        i++;
    }
    if(semaphores[i] -> sem_id == sid){
        free_waiting_procs(semaphores[i]);
        unGiveMeMemory((void *)semaphores[i]);
        semaphores[i] = NULL;
        return 0;
    }
    return -1;
}

node_pointer create_node(int pid){
    node_pointer node = (node_pointer)  giveMeMemory(sizeof(struct process_node));
    node ->pid = pid;
    node -> next = NULL;
    return node;
}

void add_waiting_proc(uint64_t lock, semaphore sem, int pid){
    
    set_state(pid, BLOCKED);
    if(sem->waiting_proc == NULL){
       sem -> waiting_proc = create_node(pid);
    }

    else{
        node_pointer aux = sem ->waiting_proc;
        while(aux -> next != NULL){
            aux = aux->next;
        }
        aux -> next = create_node(pid);
    }
}

int s_wait(int sid){
    int i = 0;
    while(i < MAX_SEMS && (semaphores[i] == NULL || semaphores[i] -> sem_id != sid)){
        i++;
    }
    if(i == MAX_SEMS){
        return -1;
    }
    else{
        int pid = get_current_pid();
        enter_region((uint64_t)&(semaphores[i]->state), semaphores[i], pid);
        return semaphores[i]->state;
    }
}

void check_blocked(uint64_t lock, semaphore sem){
    node_pointer aux = sem -> waiting_proc;
    if(aux != NULL){
        set_state(aux -> pid, READY);
        sem->waiting_proc = sem -> waiting_proc -> next;
        unGiveMeMemory((void *)aux);
        (sem->state)++;
    }
}

int s_post(int sid){
    int i = 0;
      while(i < MAX_SEMS && (semaphores[i] == NULL || semaphores[i] -> sem_id != sid)){
        i++;
    }
    if(i == MAX_SEMS){
        return -1;
    }
    else{
        leave_region((uint64_t)&(semaphores[i]->state), semaphores[i]);
        return semaphores[i]->state;

    }
}

void print_sems(){
    int i = 0;
    while(i < MAX_SEMS){
        if(semaphores[i] !=  NULL){
            ncPrint("Sem name: ");
            ncPrint(semaphores[i] -> name);
            ncNewLine();
            ncPrint("Sem id: ");
            ncPrint(semaphores[i] -> sem_id);
            ncNewLine();
            ncPrint("Sem state: ");
            (semaphores[i] -> state) == 1 ? ncPrint("LOCKED") : ncPrint("UNLOCKED");
            ncNewLine();
        }
    }
}

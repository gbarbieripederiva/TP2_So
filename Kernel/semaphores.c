
#include <semaphores.h>

#define MAX_SEMS 100





int sem_id;
semaphore semaphores[MAX_SEMS];
//initiates the array of semaphores with no sems in it
void init_sems(){
    int i=0;
    while(i < MAX_SEMS){
        semaphores[i++] = NULL;
    }
    sem_id = 0;
}

//mallocs a struct to open a sem
semaphore create_sem_struct(int name, int state){
    semaphore aux1 = (semaphore) giveMeMemory (sizeof(sem));
    aux1 -> name = name;
    aux1 ->refs = 1; //if it was created it was openned
    aux1 -> sem_id = sem_id;
    aux1 -> state = (uint64_t)state;
    aux1 -> waiting_proc = NULL;
    sem_id++;
    return aux1;
}

//it opens a semaphore with its sem id
int s_open(int name, int state){
    int j = 0;
    while((j < MAX_SEMS) && (semaphores[j] == NULL || semaphores[j]->name != name)) {
        j++;
    }
    if(semaphores[j] -> name == name){ // if it finds a semaphore with the same name it returns its sem id
    
    
        _cli();
        semaphores[j] ->refs ++;
        _sti();
    
        return semaphores[j]->sem_id;
    }
    else{ //it creates one


        int i = 0;

        while(i < MAX_SEMS && semaphores[i] != NULL){ //finds an empty position
            i++;
        }
        if(i == MAX_SEMS){ // if there is no empty space it returns -1
            return -1;
        }
        else{
            semaphores[i] = create_sem_struct(name, state); //creates a semaphore and attached to a position
            return semaphores[i] -> sem_id;
            
        }
    }
}

//it frees every process attached to the semaphore
void free_waiting_procs(semaphore sem){
    while(sem ->waiting_proc != NULL){
        node_pointer aux = sem -> waiting_proc;
        sem ->waiting_proc = sem ->waiting_proc ->next;
        unGiveMeMemory((void *)aux);
    }
}

//it closes the semaphore identified by the sem id
int s_close(int sid){
    int i = 0;
    while(i < MAX_SEMS && (semaphores[i] == NULL || semaphores[i] -> sem_id != sid)){
        i++;
    }
    if(semaphores[i] != NULL && semaphores[i] -> sem_id == sid){
        if(semaphores[i] -> refs > 1){ //opened from different processes
        
            _cli();
            (semaphores[i] -> refs)--;
            _sti();
        
        
        }
        else{ //it is openned in just 1 process
            free_waiting_procs(semaphores[i]);
            unGiveMeMemory((void *)semaphores[i]);
            semaphores[i] = NULL;
        }
        return 0;

    }
    return -1;
}


//creates the node to add a process blocked by the sem
node_pointer create_node(int pid){
    node_pointer node = (node_pointer)  giveMeMemory(sizeof(struct process_node));
    node ->pid = pid;
    node -> next = NULL;
    return node;
}

// adds a process to the queue of waiting processes
void add_waiting_proc(uint64_t lock, semaphore sem, int pid){
    
    
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
        set_state(pid, BLOCKED);
        _int20();
}

// it checks the lock and blocks if it is necessary
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

//when a semaphore is unlocked, it checks if there is a blocked process by it
void check_blocked(uint64_t lock, semaphore sem){
    node_pointer aux = sem -> waiting_proc;
    if(aux != NULL){
        set_state(aux -> pid, READY);
        sem->waiting_proc = sem -> waiting_proc -> next;
        unGiveMeMemory((void *)aux);
        (sem->state) = SEM_LOCKED;

    }
}
//unlocks the sem
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

node_pointer waiting_iterator(semaphore sem){
    node_pointer aux = sem -> waiting_proc;
    return aux;
}
//prints information of openned sems
void print_sems(){
    int i = 0;
    while(i < MAX_SEMS){
        if(semaphores[i] !=  NULL){
            ncPrint("Sem name: ");
            ncPrintDec(semaphores[i] -> name);
            ncNewLine();
            ncPrint("Sem id: ");
            ncPrintDec(semaphores[i] -> sem_id);
            ncNewLine();
            ncPrint("Sem state: ");
            (semaphores[i] -> state) == 1 ? ncPrint("LOCKED") : ncPrint("UNLOCKED");
            ncNewLine();
            node_pointer aux = waiting_iterator(semaphores[i]);
            int j = 1;
            while(aux != NULL){
                ncPrint("Waiting process");
                ncPrintDec(j);
                ncPrint(": ");
                ncPrintDec(aux->pid);
                ncNewLine();
                j++;
            }
           
        }
        i++;
    }
}




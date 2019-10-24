#include <scheduler.h>
#include <stdint.h>
#include <stdio.h>
#include <process.h>
#include <interrupts.h>



static int iterator;

procInSched procsInSched[SIZE];

void init_sched(){
    procInSched aux;
    aux.state = EMPTY;
    int i;
    for(i = 0; i < SIZE; i++){
        procsInSched[i] = aux;
    }
    processInfo halt;
    halt = create_process("halt",0,(uint64_t)&haltFunction);
    run_process(halt, HALT);
    init_iterator();
}


procInSched create_fill_struct(processInfo process, int state){
    procInSched aux;
    aux.process = process;
    aux.entered = process -> priority;
    aux.state = state;
    return aux;
}


int run_process(processInfo process, int state){
    int i = 0;
    while(i < SIZE && procsInSched[i].state != EMPTY){
        i++;
    }
    if(procsInSched[i].state == EMPTY){
        procInSched aux = create_fill_struct(process, state);
        procsInSched[i] = aux;
        return 1;
    }
    return 0;
}

void init_iterator(){
    int i = 0;
    while(i < SIZE && procsInSched[i].state != READY){
        i++;
    }
    if(i == SIZE){
        iterator = 0; //if no procInSched is ready then halt
    }
    else{
        iterator = i; }

}

//if it completes one cycle iterator goes to procInSched 0 which is halt
void next(){
    if(procsInSched[iterator].state == READY && procsInSched[iterator].entered != 0){
        procsInSched[iterator].entered--;
        return;
    }
    else {    
        procsInSched[iterator].entered = procsInSched[iterator].process -> priority; //reset entered times
        iterator++;
        int i = SIZE + 1; //counts the times it enters the loop
       
        while( procsInSched[iterator % SIZE].state != READY && i > 0){
            iterator++;
            i--;
        }
        if(i == 0){ //if i == 11 then there is no procInSched
            iterator = 0; 
        }
        iterator = iterator % SIZE;
    }

}

int kill_process(int pid){
    int i = 0;

    while(i < SIZE && procsInSched[i].process -> pid != pid ){
        i++;
    }
    if(procsInSched[i].process -> pid == pid){
        procsInSched[i].state = EMPTY;
        return 0;
    }
    return -1;
}

uint64_t scheduler(uint64_t sp){
    procsInSched[iterator].process -> sp = sp;
    next();
    return(procsInSched[iterator].process -> sp);
}

int set_state(int pid, int state){
    int curr = 0;
    while(curr < SIZE && procsInSched[curr].process ->pid != pid){
        curr++;
    }
    if(procsInSched[curr].process ->pid == pid){
        procsInSched[curr].state = state;
        return 0;
    }
    return -1;

}

int get_current_pid(){
    return procsInSched[iterator].process->pid;
}
/*
 void printTest(procInSched test){
    printf("pid = %d\n",  test.process -> pid);
    switch (test.state){
        case 0: 
                printf("BLOCKED\n");
                break;
        case 1: 
                printf("READY\n");
                break;
        case 2: 
                printf("EMPTY\n");
                break;
        case 3: 
                printf("HALT\n");
                break;

    }
    printf("Function name: %s\n", test.process ->name);
    printf("Stack Pointer: %llu\n", test.process -> sp);
    printf("Priority: %d\n", test.process -> priority);
    printf("Entered = %d\n\n", test.entered);
}


*/
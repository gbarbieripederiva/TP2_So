#include <scheduler.h>
#include <stdint.h>
#include <stdio.h>
#include <process.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <naiveConsole.h>



static int iterator = 0;
static int first_time = 1;
static int running_procs;

procInSched procsInSched[SIZE];

void halt_proc(){
    while (1)
    {
        haltFunction();
    }
    
}

void init_sched(){
    first_time = 1;
    running_procs = 0;
    procInSched aux;
    aux.state = EMPTY;
    int i;
    for(i = 0; i < SIZE; i++){
        procsInSched[i] = aux;
    }
    processInfo halt;
    halt = create_process(0,(uint64_t)halt_proc);
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
    if(running_procs >= SIZE){
        return -1;
    }
    //if not it will always find an empty space in sched
    int i = 0;
    while(i < SIZE && procsInSched[i].state != EMPTY){
        i++;
    }
    procInSched aux = create_fill_struct(process, state);
    running_procs++;
    procsInSched[i] = aux;
    return 0;
}
//it initiates the iterator in the first READY process if there arent anyone it returns 0 which is halt process
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
            if(i == 0 && procsInSched[iterator % SIZE].state != READY){ //if i == 0 then there is no procInSched
                iterator = 0; //returns halt process
            }
            iterator = iterator % SIZE;
        }

}
// kills a process if it is running it calls int 20 to switch context
int kill_process(int pid){
    int i = 0;

    while(i < SIZE && procsInSched[i].process -> pid != pid ){
        i++;
    }
    if(procsInSched[i].process -> pid == pid) {  //if i equals iterator it means it is currently running we cant remove it
        procsInSched[i].state = EMPTY;
        unGiveMeMemory(procsInSched[i].process->sp);
        unGiveMeMemory(procsInSched[i].process);
        running_procs--;
        _int20();
        
        return 0;
    }
    return -1;
}

//switch context
uint64_t scheduler(uint64_t sp){
    if(first_time == 1){
        first_time--; 
    }
    else if(sp >= 0x600000){
    procsInSched[iterator].process -> sp = sp;}
    next();
    return(procsInSched[iterator].process -> sp);
}

//set states of a program
int set_state(int pid, int state){
    int curr = 0;
    while(curr < SIZE && procsInSched[curr].process ->pid != pid){//busca el pid
        curr++;
    }
    if(procsInSched[curr].process ->pid == pid){
        procsInSched[curr].state = state;
        _int20();
        return 0;
    }
    return -1;
}
//it returns the process id of the running process
int get_current_pid(){
    return procsInSched[iterator].process->pid;
}

//prints information of a running process
void print_running_procs(){
    int i = 0;
    while(i < SIZE){
        if(procsInSched[i].state != EMPTY){
            ncNewLine();
            ncPrint("PID:");
            ncPrintDec((uint64_t)procsInSched[i].process ->pid);
            ncNewLine();
            ncPrint("PPID:");
            ncPrintDec((uint64_t)procsInSched[i].process ->ppid);
            ncNewLine();
            ncPrint("State: ");
            ncPrintDec((uint64_t)procsInSched[i].state);
            ncNewLine();
            ncPrint("Priority: ");
            ncPrintDec((uint64_t)procsInSched[i].process->priority);
            ncNewLine();
        }
        i++;
    }
}

// get current process
processInfo getCurrentProcess(){
    return procsInSched[iterator].process;
}

// set current process stdin
void setCurrentStdin(int fd){
    procsInSched[iterator].process->stdin=fd;
}
// set current process stdout
void setCurrentStdout(int fd){
    procsInSched[iterator].process->stdout=fd;
}
// set process stdin
void setStdin(int pid,int fd){
    //TODO
    if(fd<0){
        setCurrentStdin(fd);
    }
}

// set process stdout
void setStdout(int pid,int fd){
    //TODO
    if(fd<0){
        setCurrentStdout(fd);
    }
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
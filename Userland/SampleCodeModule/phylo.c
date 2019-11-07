#include <phylo.h>
#include <terminal.h>
#include <stdint.h>
#include <libasm.h>
#include <lib.h>
#define NULL 0

#define NO_OVER 50000
#define MAX_PHYLO 10
#define HUNGRY 3
#define THINKING 2
#define EATING 1
#define LEFT ((i + cant - 1) % cant)
#define RIGHT ((i + cant) % cant)
//Based on Tannenbaum

int mutex;
int print_pid;
int state[MAX_PHYLO];
int sem_id[MAX_PHYLO];
int cant;
int command;
int monitor_pid;
int pid[MAX_PHYLO];
 uint64_t info[MAX_PHYLO];
 int goOn = 1;

void phylosopher(int i);
void monitor();


void printState (){
    int h = 0;
        
            while(h < cant){
                        print("Phylosofer ");
                        printDec(h + 1);
                        print(" state: ");
                        switch (state[h])
                        {
                        case HUNGRY:
                            print("HUNGRY");
                            break;
                        case EATING:
                            print("EATING");
                            break;
                        case THINKING:
                            print("THINKING");
                            break;
                        
                        default:
                            break;
                        }
                        printAction(0);
                
                
                h++;
            }
                printAction(0);
                printAction(0);
}

void start_phylo(int phylos){
    mutex = sys_create_semaphore(12234321, SEM_UNLOCKED);

    monitor_pid = sys_create_process(0, (uint64_t) monitor);
    sys_run_process(monitor_pid, 1);
   
    cant = phylos;
    int j = 0;
   
    int g = 0;
    while (g < MAX_PHYLO)
    {
        pid[g] = -1;
        info[g] = -1;
        state[g] = THINKING;
        g++;
    }
    

    while(j < cant){
        info[j] = sys_create_process_params(0,(uint64_t)phylosopher, j, j);
        sem_id[j] = sys_create_semaphore(j + NO_OVER, SEM_LOCKED);
        sys_run_process(info[j], 1);
        pid[j] = ((processInfo) info[j]) -> pid;
        j++;
    }
    uint64_t end = getTicks() + 3 * 18;

      while(1){
        if(end < getTicks()){
            end = getTicks() + 3 * 18;
            printState();
        }
        if(goOn == 0){
            return;
        }
      }
    
    
    


}

void phylosopher(int i){
    while(1){
        waitFor(random()); //simulating thinking
        take_forks(i);
        waitFor(random()); //simulating eating
        put_forks(i);

    }
}


void monitor(){
    char buffer;
    while(1){
        sys_read_pipe(KEYBOARD, &buffer,1);
        switch (buffer)
        {
        case 'A':
        case 'a':
            if(cant == MAX_PHYLO){
                print("This program just admits");
                printDec(MAX_PHYLO);
                print("phylosophers");
                printAction(0);
            }
            else{
                sem_id[cant] = sys_create_semaphore(cant + NO_OVER, SEM_LOCKED);
                info[cant] = sys_create_process_params(0,(uint64_t) phylosopher, cant, 0);
                pid[cant] = ((processInfo) info[cant]) -> pid;
                sys_run_process(info[cant], 1);
                cant ++;
            }
            break;
        case 'S':
        case 's':
            if(cant == 0){
                print("No more philosophers to delete");
            }
            else{
                sys_sem_close(sem_id[cant - 1]);
                info[cant - 1] = NULL;
                sys_kill_process(pid[cant - 1]);
                cant --;
            }
            break;
        
        case 'Q':
        case 'q': ;
            int j = 0;
            while(j < cant){
                sys_kill_process(pid[j]);
                sys_sem_close(sem_id[j]);
                info[j] = NULL;
                j++;
            }
            goOn = 0;
            return;
        default:
            print("Not a valid command");
            printAction(0);
            break;
        }
    }
    return;
}

void take_forks(int i){
    sys_sem_wait(mutex);
    state[i] = HUNGRY;
    test(i);
    sys_sem_post(mutex);
    sys_sem_wait(sem_id[i]);
}

void put_forks(i){
    sys_sem_wait(mutex);
    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    sys_sem_post(mutex);

}

void test(int i){
    if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
        state[i] = EATING;
        sys_sem_post(sem_id[i]);
    }
}
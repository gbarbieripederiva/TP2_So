#include <phylo.h>
#include <terminal.h>
#include <stdint.h>
#include <libasm.h>
#include <lib.h>

#define NO_OVER 50000
#define MAX_PHYLO 10
#define HUNGRY 3
#define THINKING 2
#define EATING 1
#define LEFT ((i + cant - 1) % cant)
#define RIGHT ((i + cant) % cant)
//Based on Tannenbaum

int mutex;
int state[MAX_PHYLO];
int sem_id[MAX_PHYLO];
int cant;

void phylosopher(int i);


void printState (){
    int h = 0;
    while(h < cant){
            print("Phylosofer ");
            printDec(h);
            print("state: ");
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
    
        
        h++;
    }
}

void start_phylo(int phylos){
    int pid[MAX_PHYLO];
    mutex = sys_create_semaphore(12234321, SEM_UNLOCKED);
   
    cant = phylos;
     printDec(cant);
    int j = 0;
    uint64_t info[MAX_PHYLO];
    int g = 0;
    while (g < MAX_PHYLO)
    {
        pid[g] = -1;
        info[g] = -1;
        state[g] = -1;
        g++;
    }
    

    while(j < cant){
        info[j] = sys_create_process_params(0,(uint64_t)phylosopher, j, j);
        sem_id[j] = sys_create_semaphore(j + NO_OVER, SEM_LOCKED);
        sys_run_process(info[j], 1);
        pid[j] = ((processInfo) info[j]) -> pid;
        j++;
    }
    uint64_t start = getTicks();
    uint64_t diference = 3 * 30000;
    while (1)
    {   
        
        if((start - getTicks()) > diference){
            start = getTicks();
            printState();
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
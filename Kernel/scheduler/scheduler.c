#include "../mmu/memoryManager.h"
#include "../include/null.h"
#define READY 1
#define BLOCKED 0

struct node{
    int pid;
    int state;
    uint64_t sp;
    struct node  * next;
};

struct header{
    struct node *first;
    struct node *iter;
};

static struct header list;

struct header init(){
    list.first=NULL;
    list.iter = NULL;
    return list;
}

void addProcess(struct node *toAdd) {
    if(list.first == NULL){
        toAdd->next = toAdd;
        list.first = toAdd;
    }
    else{
        struct node *aux = list.first->next;
        list.first->next = toAdd;
        toAdd -> next = aux;
    }
}



struct node *createFillStruct(int pid, int state, uint64_t sp){
    struct node *toFill = (struct node *)giveMeMemory(sizeof(struct node));
    toFill -> pid = pid;
    toFill -> state = state;
    toFill -> sp = sp;
    return toFill;
}
   
 void printTest(struct node *test){
    printf("%d\n%d\n%llu\n\n", test->pid, test -> state, test->sp);
}

void init_iter(struct header *list){
    list -> iter = list ->first;
}

void next(struct header *list){
    list -> iter = list -> iter -> next;
}



uint64_t scheduler(uint64_t sp){
    list.iter->sp = sp;
    struct node *infLoop = list.iter;
    list.iter = list.iter->next;
    while(list.iter->sp == BLOCKED){
        next(&list);
        if(list.iter == infLoop){
            printf("inf loop\n");
            return -1;
        }
    }
    return list.iter->sp;
}


/*
int main(){

    int pid = 1, state = READY;
    uint64_t sp = 1;
    struct node * test = createFillStruct(pid,state,sp);
    init();
    
    addProcess(test);

     int pid2 = 2, state2 = BLOCKED;
    uint64_t sp2 = 2;

    struct node * test2= createFillStruct(pid2,state2,sp2);
    addProcess(test2);

        int pid3 = 3, state3 = READY;
    uint64_t sp3 = 3;

    struct node * test3= createFillStruct(pid3,state3,sp3);
    addProcess(test3);


    init_iter(&list);

    uint64_t aux = 7;
    scheduler(aux);


    printTest(list.iter);
    printTest(list.iter->next);
    printTest(list.iter->next->next);
    return 0;  
}

 */
#include <stdint.h>
#include "include/null.h"

#define STARTING_MEM_LOCATION 0x600000
#define INITIAL_MEM_SIZE 100000


typedef struct memBlock{
    uint64_t startLocation;
    uint64_t size;
} memBlock;

static uint64_t memSize;

static void swap(memBlock * block1, memBlock * block2){
    memBlock aux = *block1;
    block1->startLocation = block2->startLocation;
    block1->size = block2->size;
    block2->startLocation = aux.startLocation;
    block2->size = aux.size;
}

void initializeMemory(){
    memSize = INITIAL_MEM_SIZE - sizeof(memBlock);
    memBlock * aux = STARTING_MEM_LOCATION + memSize;
    aux->startLocation = NULL;
}

void * giveMeMemory(uint64_t size){
    memBlock * lowest = STARTING_MEM_LOCATION + INITIAL_MEM_SIZE - sizeof(memBlock);
    uint64_t location = STARTING_MEM_LOCATION;

    while(lowest->startLocation != NULL){

        if(lowest->startLocation - location >= size){
            void * retVal = (void *) location;

            memBlock aux = *lowest;
            lowest->startLocation = location;
            lowest->size = size;
            lowest -= sizeof(memBlock);

            while(aux.startLocation != NULL){
                swap(&aux, lowest);
                lowest -= sizeof(memBlock);
            }
            lowest->startLocation = NULL;
            memSize -= sizeof(memBlock);
            return retVal;
            //return statement
        }

        location = lowest->startLocation + lowest->size;
        lowest -= sizeof(memBlock);

    }
    //si llegue aca, entonces no hay memoria entre dos bloques o entre principio y primer bloque
    if(STARTING_MEM_LOCATION + memSize - location < size){
        return (void *)NULL;
    }

    lowest->startLocation = location;
    lowest->size = size;
    memSize -= sizeof(memBlock);
    lowest -= sizeof(memBlock);
    lowest->startLocation = NULL;
    return (void *)location;

}

#include "../include/memoryManager.h"
#include "../include/null.h"
#include <stdint.h>

#define STARTING_MEM_LOCATION 0x600000
#define INITIAL_MEM_SIZE 100000


typedef struct memBlock{
    uint64_t startLocation;
    uint64_t size;
} memBlock;


//memSize is changed dinamically to account for the size needed to keep track of the many memory blocks
static uint64_t memSize;
static memBlock * greatest;

static void swap(memBlock * block1, memBlock * block2){
    memBlock aux = *block1;
    block1->startLocation = block2->startLocation;
    block1->size = block2->size;
    block2->startLocation = aux.startLocation;
    block2->size = aux.size;
}

//initializeMemory() must be called once before using the memory Manager. Failure to do so will most likely result in segFault

void initializeMemory(){

    //we need to account for the size of the memBlock struct at the end used to represent an "end of array"
    memSize = INITIAL_MEM_SIZE - sizeof(memBlock);

    //the last memBlock in this array has its startLocation set to NULL to signify an end of the array
    greatest = (memBlock *) (STARTING_MEM_LOCATION + memSize);
    greatest->startLocation = NULL;
}

//something like a malloc(). Traverses the memory linearly using a first fit algorithm.
//Keeps track of different memory blocks with a memBlock "array" at the end, which grows backwards

void * giveMeMemory(uint64_t size){

    //"lowest" would be the allocated chunk of memory closest to the start of the memory such that the new block we want to allocate 
    //is closer to the start of the memory than "lowest" 
    memBlock * lowest = (memBlock *) (STARTING_MEM_LOCATION + INITIAL_MEM_SIZE - sizeof(memBlock));

    //"location" is the current starting point for a new chunk of memory
    uint64_t location = STARTING_MEM_LOCATION;

    while(lowest->startLocation != NULL){

        //the condition checked is whether there is enough space for the new memory chunk
        //but also whether between the last chunk and the "end" of the memBlock array there is enough space for a new memBlock struct

        if(lowest->startLocation - location >= size && (uint64_t) greatest - (greatest->startLocation + greatest->size) >= sizeof(memBlock) ){
            void * retVal = (void *) location;

            //since the memBlock array is ordered, we need to save the new memBlock where lowest is currently,
            //and then shift the rest of the array one position 

            memBlock aux;
            aux.startLocation = location;
            aux.size = size;

            while(aux.startLocation != NULL){
                swap(&aux, lowest);
                lowest -= sizeof(memBlock);
            }

            //add the "end of array" marker and return
            lowest->startLocation = NULL;
            memSize -= sizeof(memBlock);
            return retVal;
        }

        location = lowest->startLocation + lowest->size;
        lowest -= sizeof(memBlock);

    }

    //if the program reaches this point, then a "lowest" block does not exist. We check whether there is enough
    //space between the end of the last allocated chunk and the end of our memory (the end being the last
    //memBlock struct in our backwards array at the end)

    if(STARTING_MEM_LOCATION + memSize - location < size + sizeof(memBlock)){
        //there isn't enough space
        return (void *)NULL;
    }

    //we set this chunk of memory as allocated by saving it in the last position of the memBlock array
    lowest->startLocation = location;
    lowest->size = size;

    //decrement memory size to account for the new memBlock created
    
    memSize -= sizeof(memBlock);

    //indicate the end of the memBlock array

    lowest -= sizeof(memBlock);
    lowest->startLocation = NULL;
    return (void *)location;

}

//Equivalent to free()
//This algorithm is O(n) where n is the total number of memory chunks. Could be upgraded to O(log n) very easily,
//but I doubt we will be managing many chunks at any time

int unGiveMeMemory(void * location){

    uint64_t castLocation = (uint64_t) location;
    //"lowest" is at each step the chunk of memory closest to the start of the memory that we haven't checked yet
    memBlock * lowest = (memBlock *) (STARTING_MEM_LOCATION + INITIAL_MEM_SIZE - sizeof(memBlock));

    while(lowest->startLocation != NULL){
        if(lowest->startLocation > castLocation){
            //there is no memory chunk starting at the specified location
            return -1;
        }
        if (lowest->startLocation == castLocation){
            //move array right and return 0
            memBlock * next = lowest - sizeof(memBlock);
            while(next->startLocation != NULL){
                lowest->startLocation = next->startLocation;
                lowest->size = next->size;
                lowest -= sizeof(memBlock);
                next -= sizeof(memBlock);
            }
            //add end of array marker at the end
            lowest->startLocation = NULL;

            //the memory is now sizeof(memBlock) bigger
            memSize += sizeof(memBlock);
            return 0;
        }
        //the specified memory chunk could be further ahead
        lowest -= sizeof(memBlock);
    }
    //there is no memory chunk starting at the specified location
    return -1;
}
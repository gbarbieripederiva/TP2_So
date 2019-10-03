#include <stdint.h>
#include "../include/null.h"

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

        //chequear tambien espacio suficiente para un nuevo memBlock al final

        if(lowest->startLocation - location >= size && (uint64_t) greatest - (greatest->startLocation + greatest->size) >= sizeof(memBlock) ){
            void * retVal = (void *) location;

            //since the memBlock array is ordered, we need to save the new memBlock where lowest is currently,
            //and then shift the rest of the array one position 

            memBlock aux = *lowest;
            lowest->startLocation = location;
            lowest->size = size;
            lowest -= sizeof(memBlock);

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
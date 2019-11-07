#include "../include/memoryManager.h"
#include "../include/null.h"

#define STARTING_MEM_LOCATION 0x600000
#define INITIAL_MEM_SIZE 131072  //enough for 128 1K-sized blocks
#define CHUNK_SIZE 1024
#define CHUNKS (INITIAL_MEM_SIZE / CHUNK_SIZE)

#define FREE 0
#define PARTIALLY 1
#define OCCUPIED 2

/* The chunk at position 0 in the status array has size INITIAL_MEM_SIZE
   Then, the chunk at position k in the array might be split into two chunks of half the size,
   which are in the 2k + 1 and 2k + 2 positions of the same array 
 */

static uint8_t status[CHUNKS] = {FREE}; //all chunks start as free


//this function does nothing. It's there to keep compatibility with memoryManager, although it could be deleted from there as well
//this is basically legacy
void initializeMemory(){
    
}

static void * giveMeMemoryWrapped(uint64_t size, uint32_t chunk, uint64_t currentSize, uint64_t memLocation){

    //if a smaller chunk can be allocated, we'll do exactly that

    if(size <= currentSize / 2 && 2 * chunk + 1 < CHUNKS){

        void * retVal = (void *)NULL;

        /*
            we want to first check for any potential partially occupied smaller chunks. This reduces wasted space
            (think about allocating first a small piece of memory and then a much bigger one. If we can allocate on a 
            partially occupied chunk, then free chunks stay free and are bigger)
        */ 

        if(PARTIALLY == status[2 * chunk + 1]){
            retVal = giveMeMemoryWrapped(size, 2 * chunk + 1, currentSize / 2, memLocation);
        }

        if((void *) NULL == retVal && PARTIALLY == status[2 * chunk + 2]){
            retVal = giveMeMemoryWrapped(size, 2 * chunk + 2, currentSize / 2, memLocation + currentSize / 2);
        }

        //we reach here with a NULL retVal if either no smaller chunk is partially occupied, or they both are but can't fit the new memory
        //if the left one is free, we try there. Else if the right one is free, we try there. Else we return NULL.

        if((void *) NULL == retVal){
            if(FREE == status[2 * chunk + 1]){
                retVal = giveMeMemoryWrapped(size, 2 * chunk + 1, currentSize / 2, memLocation);
            }
            else if(FREE == status[2 * chunk + 2]){
                retVal = giveMeMemoryWrapped(size, 2 * chunk + 2, currentSize / 2, memLocation + currentSize / 2);
            }
        }

        /*
            if retVal is not NULL, it means we updated this chunk's children halves, so we need to update the status of 
            this chunk as well. If either of the halves is partially occupied, or if one is free and the other one is
            occupied, then this chunk is partially occupied. Is both are occupied, this chunk is occupied.
            In another words, if retVal is not NULL then this chunk is either partiallr occupied or occupied.
            Since it's occupied only if both havles are occupied, we create a very simple if expression
        */ 

       if(retVal != (void *) NULL){
           if(OCCUPIED == status[2 * chunk + 1] && OCCUPIED == status[2 * chunk + 2]){
               status[chunk] = OCCUPIED;
           }
           else{
               status[chunk] = PARTIALLY;
           }
       }

       return retVal;

    }

    //if we can't allocate a smaller chunk, and this one is partially occupied, then there's no way this chunk can be used

    if(PARTIALLY == status[chunk]){
        return (void *) NULL;
    }

    //this chunk is free, and there are no smaller chunks that can be allocated

    status[chunk] = OCCUPIED;
    return memLocation;

}

//Equivalent to malloc()

void * giveMeMemory(uint64_t size){
    if(OCCUPIED == status[0] || size > INITIAL_MEM_SIZE){ //either the space requested is too big or the memory is completely occupied
        return (void *) NULL;
    }
    return giveMeMemoryWrapped(size, 0, INITIAL_MEM_SIZE, STARTING_MEM_LOCATION);
}


//I'm not commenting this. Imagine a binary tree, in which every node is ordered in inorder. 
//The location of root is 0, and for every node the left child has the same location, and the right child has the location
//(parentLocation + size/2^h) where h is the height of the current node. 

static uint32_t findFirstChunk(uint64_t location){
    location -= STARTING_MEM_LOCATION; //normalize to 0, as if it was an offset
    uint32_t chunk = 0;
    uint64_t size = INITIAL_MEM_SIZE;
    uint64_t currentLocation = 0;
    while(chunk < CHUNKS){
        if(location == currentLocation){
            return chunk;
        }
        size /= 2;
        if(location >= currentLocation + size){
            currentLocation += size;
            chunk = 2 * chunk + 2;
        }
        else{
            chunk = 2 * chunk + 1;
        }
    }
    //invalid location
    return -1;
}

//once we finde a location, the current node and its left child, and that child's left child and so on, all share the same location
//as soon as we find a node that is either a leaf or its left child is free, it means we are on the node that was allocated
static uint32_t findOccupiedChunk(uint32_t chunk){

    if(FREE == status[chunk]){
        return -1;
    }
    if(2 * chunk + 1 >= CHUNKS || FREE == status[2 * chunk + 1]){
        status[chunk] = FREE;
        return chunk;
    }
    return findOccupiedChunk(2 * chunk + 1);

}

int unGiveMeMemory(void * location){
    uint32_t chunk = findFirstChunk(location);
    if(chunk < 0){
        return -1;
    }
    chunk = findOccupiedChunk(chunk);
    if(chunk < 0){
        return -1;
    }
    status[chunk] = FREE;
    while(chunk != 0){
        //move to the parent
        chunk = (chunk - 1) / 2;
        if(OCCUPIED == status[chunk]){
            status[chunk] = PARTIALLY;
        }
        else if(FREE == status[2 * chunk + 1] && FREE == status[2 * chunk + 2]){
            status[chunk] = FREE;
        }
    }
    return 0;
}


void print_mem(){
    //TODO
}
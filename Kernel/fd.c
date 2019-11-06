#include <stdint.h>
#include <null.h>
#include <memoryManager.h>
#include <semaphores.h>
#define BUFFER_SIZE 2048
#define FD_AMOUNT 40
#define MUTEX 0
#define ABLE_TO_READ 1




int fd_id = 0; 
typedef struct fd
{
    int name;
    char buffer[BUFFER_SIZE];
    int fd_id;
    int count;
    int read_index;
    int write_index;
    int sems_id[2];

} fd;

fd * fds[FD_AMOUNT];


void init_fds(){
    fd_id = 0;
    int i = 0;
    while(i < FD_AMOUNT){
        fds[i] = NULL;
        i++;
    }

}

fd * create_fd_struct(int pos, int name){
    fd * aux = (fd *) giveMeMemory(sizeof(fd));
    aux -> name = name;
    aux -> fd_id = fd_id;
    aux -> read_index = 0;
    aux -> write_index = 0;
    aux -> sems_id[MUTEX] = s_open(pos, SEM_UNLOCKED);
    aux -> sems_id[ABLE_TO_READ] = s_open(pos + FD_AMOUNT, SEM_LOCKED);
    fd_id++;
    return aux;

}


int give_me_fd(int name){
    int i = 0;
    while(i < FD_AMOUNT && (fds[i] == NULL || fds[i] -> name != name)){
        i++;
    }
    if(fds[i] != NULL && fds[i] == name){
        return fds[i]->fd_id;
    }

    else{
        int j = 0;
        while(j < FD_AMOUNT && fds[j] != NULL){
            j++;
        }

        if(j == FD_AMOUNT){
            return -1;
        }
            fds[j] = create_fd_struct(j, name);
            return fds[j] -> fd_id;
    }
    
}


int fd_write(int fd, char *str, int size){
    int aux_size = size;
    int pos = 0;
    while(pos < FD_AMOUNT && (fds[pos] == NULL || fds[pos] -> fd_id != fd)){
        pos++;
    }
    if(pos == FD_AMOUNT){
        return -1;
    }


    s_wait(fds[pos] ->sems_id[MUTEX]);

    int ite = 0;
    while(aux_size != 0){
        fds[pos] -> buffer[fds[pos] ->write_index % BUFFER_SIZE] = str[ite];
        (fds[pos] -> write_index)++;
        (fds[pos] -> count) ++;
        ite++;
        aux_size--;
    }

    fds[pos] -> write_index = fds[pos] -> write_index % BUFFER_SIZE;

    s_post(fds[pos]->sems_id[MUTEX]);
    s_post(fds[pos]->sems_id[ABLE_TO_READ]);


    return 0;
}

int fd_read(int fd, char *dest , int maxSize){

    
    int pos = 0;
    while(pos < FD_AMOUNT && (fds[pos] == NULL || fds[pos] -> fd_id != fd)){
        pos++;
    }
    if(pos == FD_AMOUNT){
        return -1;
    }
    if(fds[pos] -> count == 0){ 
    s_wait(fds[pos] -> sems_id[ABLE_TO_READ]);}

    s_wait(fds[pos] -> sems_id[MUTEX]);

    int j = 0;

    while(j < maxSize && fds[pos] -> count != 0){
        dest[j] = fds[pos] -> buffer[fds[pos] -> read_index % BUFFER_SIZE];
        (fds[pos] -> read_index) ++;
        (fds[pos] -> count)--;
        j++;
    }
    if( j == maxSize){
        dest[j] = 0;
    }

    fds[pos] -> read_index = fds[pos] -> read_index % BUFFER_SIZE;

    s_post(fds[pos] -> sems_id[MUTEX]);
    return 0;

}



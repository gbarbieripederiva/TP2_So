#include <pipes.h>
#include <semaphores.h>
#include <null.h>
#include <fd.h>
#define MAX_PIPES 100


int pipes[MAX_PIPES];

int pipe_open(int name){
    return give_me_fd(name, 1);
}

int pipe_write(int fd, char * buff,int size){
    return fd_write(fd, buff, size);
}

int pipe_read(int fd,char * buff, int size){
    return fd_read(fd, buff, size);
}

void pipe_close(int num){
    fd_close(num);
}

void print_pipes(){
    print_only_pipes();
}

/*


typedef struct Pipe
{
    //buffer de escritura
    char pipeBuff[SIZEOFBUFF];

    //puntero de lectura
    int readP;
    int writeP;

    //semaforo de lectura
    int readSem;

    // mutex para la memoria
    int buffSem;

    //referencias al pipe de la lectura
    int readRefs;

    //referencias al pipe en la escritura
    int writeRefs;

    //espacio libre
    int count;
} Pipe;

typedef Pipe * PipeP;

static PipeP pipes[AMOUNTOFPIPES];

void create_pipe(int pos){
    pipes[pos]=giveMeMemory(sizeof(Pipe));
    //pipes[pos]->readSem=s_open();// SEMAPHORE
    //pipes[pos]->buffSem=s_open();// SEMAPHORE
    s_post(pipes[pos]->buffSem); // SEMAPHORE
    pipes[pos]->readRefs=1;
    pipes[pos]->writeRefs=1;
}

int pipe_open(int *names){
    //si esta afuera del array busca un lugar dentro del array
    int i;
    for (i = 0; i < AMOUNTOFPIPES; i++)
    {
        if(pipes[i]==NULL){
            //crea el pipe y retorna el nombre
            create_pipe(i);
            names[0]=i;
            names[1]=i+AMOUNTOFPIPES;
            return 1;
        }
    }
    //si no encuentra retorna -1
    return -1;
    
}

void pipe_close(int num){
    // checkear que este en el rango de los numeros razonables
    if(num>=0 && num<AMOUNTOFPIPES*2){
        //checkear si es de escritura y no es null
        if(num>AMOUNTOFPIPES && pipes[num-AMOUNTOFPIPES]!=NULL){
            //cambiar num
            num-=AMOUNTOFPIPES;
            //si tiene mas de una ref de escritura restasela
            if(pipes[num]->writeRefs>1){
                pipes[num]->writeRefs--;
                //si no quedan referencias de escritura deja que corran los read
                if(pipes[num]->writeRefs<1){
                    s_post(pipes[num]->readSem);
                }
            }
            //si tiene una sola referencia de escritura
            //y menos de una de lectura borralo(cerra el semaforo y libera memoria)
            else if(pipes[num]->readRefs<1){
                s_close(pipes[num]->readSem);//SEMAPHORE
                unGiveMeMemory(pipes[num]);
                pipes[num]=0;
            }
        }
        //sino, es de lectura. Checkear si no es null
        else if(pipes[num]!=NULL){
            //si tiene mas de una ref de lectura restasela
            if(pipes[num]->readRefs>1){
                pipes[num]->readRefs--;
            }
            //si tiene una sola referencia de lectura
            //y menos de una de escritura borralo(cerra el semaforo y libera memoria)
            else if(pipes[num]->writeRefs<1){
                s_close(pipes[num]->readSem);//SEMAPHORE
                unGiveMeMemory(pipes[num]);
                pipes[num]=NULL;
            }
        }
    }
}

int pipe_write(int fd, char * buff,int size){
    // si no es un fd valido retorna error
    if(fd<AMOUNTOFPIPES || fd>AMOUNTOFPIPES*2 || size<1 || pipes[fd]==0){
        return -2;
    }
    // si el size es demasiado retorna error
    if(size>SIZEOFBUFF){
        return -1;
    }
    // si no hay suficiente espacio...
    if(size+pipes[fd]->count>=SIZEOFBUFF){
        //TODO: implentar bloqueo
        return -1;
    }
    fd=fd-AMOUNTOFPIPES;

    //TODO: forzar sem a estar bloqueado
    s_wait(pipes[fd]->buffSem);//SEMAPHORE
    
    // copio el string desde el puntero de escritura hasta que termine de copiar
    // o escriba todo el buffer

    // NO COPIO EL 0 FINAL YA QUE ME GUARDO EL ESPACIO OCUPADO
    int i;
    for(i=0;i<SIZEOFBUFF && buff[i]!=0;i++){

        pipes[fd]->pipeBuff[(pipes[fd]->writeP+i)%SIZEOFBUFF]=buff[i];

    }
    //actualizo el puntero de escritura
    pipes[fd]->writeP=(pipes[fd]->writeP+i)%SIZEOFBUFF;

    //actualizo la cantidad de caracteres
    pipes[fd]->count+=size;

    //postea
    s_post(pipes[fd]->readSem);//SEMAPHORE    
    s_post(pipes[fd]->buffSem);//SEMAPHORE
    return i;
}

int pipe_read(int fd,char * buff, int size){
    //checkear que haya al menos espacio para el 0
    if(fd<0 || fd>AMOUNTOFPIPES || size<1 || pipes[fd]==0){
        return -2;
    }
    //wait del semaforo de lectura primero y luego del mutex de memoria
    s_wait(pipes[fd]->readSem);//SEMAPHORE
    s_wait(pipes[fd]->buffSem);//SEMAPHORE

    //End of pipe
    if(pipes[fd]->count==0){
        s_post(pipes[fd]->readSem);//SEMAPHORE
        s_post(pipes[fd]->buffSem);//SEMAPHORE
        return -1;
    }
    //copio hasta size-1 o count lo que sea mas chico
    int i;
    for ( i = 0; i < size-1 && i < pipes[fd]->count ; i++)
    {
        buff[i]=pipes[fd]->pipeBuff[(i+pipes[fd]->readP)%SIZEOFBUFF];
    }

    //agrego el 0 final
    buff[i]=0;

    //sete size en count-size o 0 el que sea mayor
    pipes[fd]->count=pipes[fd]->count>(size-1)?pipes[fd]->count-(size-1):0;

    //si quedan cosas por leer vuelvo a liberar el semaforo de lectura
    if(pipes[fd]->count>0){
        s_post(pipes[fd]->readSem);//SEMAPHORE
    }else if(pipes[fd]->writeRefs<1){
        s_post(pipes[fd]->readSem);//SEMAPHORE
    }
    //habilito el mutex de vuelta
    s_post(pipes[fd]->buffSem);//SEMAPHORE
}


//funciones wrapper por si queremos cambiar como se retorna
void pipe_print(char * str){
    ncPrint(str);
}
void pipe_print_dec(int dec){
    ncPrintDec(dec);
}

void pipe_print_newline(){
    ncNewLine();
}
//---------------------------------------------------------

void print_pipes(){
    //recorre el array de pipes
    for(int i=0;i<AMOUNTOFPIPES;i++){
        //si existe
        if(pipes[i]!=0){
            //printea su numero y si esta listo para ser leido
            pipe_print("Pipe num:");
            pipe_print_dec(i);
            pipe_print_newline();
            pipe_print(pipes[i]->count>0?"Unlocked":"Locked");
        }
    }
}
*/
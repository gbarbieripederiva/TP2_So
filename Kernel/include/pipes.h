#ifndef PIPES_H
#define PIPES_H

// Tamaño maximo del buffer de un pipe
#define SIZEOFBUFF 300
// Cantidad de pipes
#define AMOUNTOFPIPES 40

// Abre el pipe y retorna el "fd" que se usara
// para la lectoescritura del pipe
int pipe_open(int *names);

// Cierra el pipe abierto
void pipe_close(int num);

/*
*Escribe al pipe( solo si es menos que el tamaño maximo del buff)
*retorna -2 si hubo un error con el file descriptor y -1 si hubo
*error con el size
*/
int pipe_write(int fd, char * buff,int size);
/*
*Lee del pipe size bytes o lo que haya en el buffer
*Retorna -2 si hubo un problema con el fd y -1 
*si se cerro el pipe y no hay mas para leer
*/
int pipe_read(int fd,char * buff, int size);

// print pipes info
void print_pipes();


#endif
#ifdef FD_H
#define FD_H

void init_fds();
int give_me_fd(int name);
int fd_write(int fd_id , char *str, int size) ;
int fd_read(int fd, char *dest , int maxSize);




#endif
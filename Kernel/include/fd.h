#ifndef FD_H
#define FD_H

int give_me_fd(int name);
int fd_write(int fd, char *str, int size);
int fd_read(int fd, char *dest , int maxSize);

#endif
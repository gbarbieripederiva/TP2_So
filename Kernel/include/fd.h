#ifndef FD_H
#define FD_H
#define KEYBORD 0 

int give_me_fd(int name, int pipe);
int fd_write(int fd, char *str, int size);
int fd_read(int fd, char *dest , int maxSize);
int fd_close(int fd);
void print_fds();
void print_only_pipes();

#endif
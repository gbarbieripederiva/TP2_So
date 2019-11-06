#ifndef __INTERRUPTACTIONS80__
#define __INTERRUPTACTIONS80__
#include <stdint.h>
#include <process.h>

// the int 80h dispatcher
uint64_t interruptAction80Dispatcher(uint64_t callNumber, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);
void sys_read_function(char *buffer, int count);				//SYS_CALL 0
void sys_write(char *buffer, int count);						//SYS_CALL 1
void sys_put_char(char c);										//SYS_CALL 2
void sys_print_action(int action);                              //SYS_CALL 3
void sys_write_number(int number, int option);                  //SYS_CALL 4
void sys_change_sound(int option, uint16_t frequency);          //SYS_CALL 5
void sys_put_char_fixed(uint16_t x, uint16_t y, uint8_t character); //SYS_CALL 6               
uint64_t sys_get_ticks();										//SYS_CALL 10
char sys_get_last_in();											//SYS_CALL 11
uint8_t sys_get_clock_info(int option);                         //SYS_CALL 12
void sys_screen(uint64_t option, uint64_t arg1, uint64_t arg2); //SYS_CALL 20
uint64_t sys_mem_get(long size);                          //SYS_CALL 45
int sys_mem_free(uint64_t chunk);                           //SYS_CALL 46
uint64_t sys_create_process(int priority, uint64_t process); //SYSCALL 47
int sys_run_process(uint64_t process, int state); //SYSCALL 48
int sys_kill_process(int pid); //SYSCAL 49
int sys_get_pid();//SYSCALL 50
void sys_print_running_procs(); //SYSCALL 51
int sys_set_priority(int pid, int priority); //SYSCALL 52
int sys_set_state(int pid, int priority);//SYSCALL 53
uint64_t sys_create_process_params(int priority, uint64_t rip, uint64_t arg1, uint64_t arg2); //SYSCALL 54
int sys_set_stdin(int pid, int fd);//SYSCALL 54
int sys_set_stdout(int pid, int fd);//SYSCALL 55
int sys_create_semaphore(int name, int state); //SYSCALL 60
int sys_sem_close(int sid); //SYSCALL 61
int sys_sem_post(int sid); //SYSCALL 62
int sys_sem_wait(int sid); //SYSCALL 63
void sys_print_sems(); //SYSCALL 64
int sys_open_pipe(int name); //SYSCALL 75
void sys_close_pipe(int fd); //SYSCALL 76
int sys_write_pipe(int fd, char * buffer, int size); //SYSCALL 77
int sys_red_pipe(int fd, char * buffer, int size); //SYSCALL 78
void sys_print_pipe();




#endif
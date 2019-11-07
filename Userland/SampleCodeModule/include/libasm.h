
#ifndef __LIBASMUSERLAND___
#define __LIBASMUSERLAND__

#include <stdint.h>
#define SEM_LOCKED 1
#define SEM_UNLOCKED 0
#define KEYBOARD 0

#define PROC_BLOCK 0
#define PROC_RUNNING 1

typedef struct process{
    int ppid;
    int pid;
    char * name;
    int priority;
    uint64_t stack_end;
    uint64_t sp;
    int fd[2];
} process;

typedef struct process * processInfo;

//SYS_CALL 0 copy  count chars from the keyboard or waits until it has them, to the buffer.
void sys_read_call(uint64_t buffer, uint64_t count);

//SYS_CALL 1 writes in screen content from buffer.
void sys_write_call(uint64_t buffer, uint64_t count);

//SYS_CALL 2 prints a char in the screen.
void sys_put_char_call(uint64_t c);

//SYS_CALL 3 prints an action based on the number.
void sys_print_action_call(uint64_t action);

//SYS_CALL 4 prints a number based on the option
void sys_print_number_call(uint64_t number, uint64_t option);

//SYS_CALL 5 turns off beep(if option = 0), o turns on beep(option = 1) with a frequency. 
void sys_change_sound_call(uint64_t option, uint64_t frequency);

//SYS_CALL 6 puts char in a fixed position
void sys_put_char_fixed_call(uint64_t character, uint64_t x, uint64_t y);

//SYS_CALL 10, get ticks from start time
int sys_get_ticks_call();

//SYS_CALL 11, get last input form keyboard and consume it
char sys_get_last_in_call();

//SYS_CALL 12, gets the seconds, minutes or hours.
uint64_t sys_get_clock_call(uint64_t option);

//SYS_CALL 20 ,for everything screen based
void sys_screen(uint64_t option, uint64_t arg1, uint64_t arg2);

//SYS_CALL 45, for memory management
void * sys_get_memory(long size);

//SYS_CALL 46, for memory free
int *sys_mem_free(void * chunk);

//SYS_CALL 47, to create a process
uint64_t sys_create_process(int priority, uint64_t process);

//SYS CALL 48, to put a process into the scheduler
int sys_run_process(uint64_t process, int state);

//SYS CALL 49, to kill a running process
int sys_kill_process(int pid);

//SYS CALL 50, to get pid
int sys_get_pid();

//SYS CALL 51 to print process information
void sys_print_running_procs();

//SYS CALL 52 to set priority
int sys_set_priority(int pid, int priority);

//SYS CALL 53 to set state to a process
int sys_set_state(int pid, int priority);

//SYS CALL 54 to create a process with parameters
uint64_t sys_create_process_params(int priority, uint64_t rip, uint64_t arg1, uint64_t arg2);

int sys_set_stdin(uint64_t pid, int fd);

int sys_set_stdout(uint64_t pid, int fd);

int sys_get_stdin();

int sys_get_stdout();

//SYS CALL 60, to create a sempahore
int sys_create_semaphore(int nombre, int state);

//SYS CALL 61, to close a semaphore
int sys_sem_close(int sid);

//SYS CALL 62, to post a semaphore
int sys_sem_post(int sid);

//SYS CALL 63, to wait for a semaphore
int sys_sem_wait(int sid);

//SYS CALL 64, to print openned sems
void sys_print_sems();

//SYS CALL 75, to open a pipe
int sys_open_pipe(int fd);

//SYS CALL 76, to close a pipe
void sys_close_pipe(int fd);

//SYS CALL 77 to write in a pipe
int sys_write_pipe(int fd, char * buffer, int size); //SYSCALL 77

//SYS CALL 78 to read from a pipe
int sys_read_pipe(int fd, char * buffer, int size); //SYSCALL 78

//SYS CALL 79 to print state of a pipe
void sys_print_pipe();


void testchoi(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);


void sys_print_mem();
#endif
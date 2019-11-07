#include <interrupt80.h>
#include <interruptActions.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <keyboard.h>
#include <interrupts.h>
#include <drivers/videoDrivers.h>
#include <lib.h>
#include <memoryManager.h>
#include <process.h>
#include <scheduler.h>
#include <semaphore.h>
#include <pipes.h>
//Software interrupt used for interaction between user and kernel space
//order of registers in standard rdi -> call number,rsi -> arg1 ,rdx -> arg2 ,rcx -> arg3
uint64_t interruptAction80Dispatcher(uint64_t callNumber, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4)
{

	switch (callNumber)
	{
	//sys_read. Just reads from the keyboard and copies to the buffer.
	case 0:
		sys_read_function((char *)arg1, (int)arg2);
		break;
	// sys_write: writes stdout the content of buffer
	case 1:
		sys_write((char *)arg1, (int)arg2);
		break;
	//sys_put_char: prints a char in the screen.
	case 2:
		sys_put_char((char)arg1);
		break;
	//sys_print_action: prints an action (enter or backspace)
	case 3:
		sys_print_action((int)arg1);
		break;
	case 4:
		sys_write_number((int)arg1,(int) arg2);
		break;
	//sys_change_sound: turns on beep(with a frequency) or turns it off
	case 5: 
		sys_change_sound((int)arg1, (uint16_t)arg2);
		break;
	//sys_call_put_char_fixed: puts char in a designated position.
	case 6:
		sys_put_char_fixed((uint16_t)arg1, (uint16_t)arg2, (uint8_t)arg3);
		break;
	//sys_get_ticks
	case 10:
		return (uint64_t) sys_get_ticks();
		break;
	//sys_get_last_in: peek the last char entered in the keyboard
	case 11:
		return (uint64_t) sys_get_last_in();
		break;
	//sys_get_clock_info: gets the the seconds, minutes or hours.
	case 12:
		return (uint64_t) sys_get_clock_info((int)arg1);
		break;
	//sys_screen: used for managing screen from userland
	case 20:
		sys_screen(arg1, arg2, arg3);
		break;
	//sys_mem_get: get memory
	case 45:
		return (uint64_t) sys_mem_get((long)arg1);
		break;
	//sys_mem_free: frees memory
	case 46:
		return (int) sys_mem_free((uint64_t) arg1);
		break;
	//sys_create_process: Creates and registers new process
	case 47:
		return (uint64_t) sys_create_process((int) arg1, (uint64_t)arg2); 
		break;
	//sys_run_process: Puts process into scheduler with state READY, BLOCKED, HALT
	case 48:
		return (int) sys_run_process((uint64_t) arg1, (int) arg2);
		break;
	//sys_kill_process: stops iterating process from scheduler
	case 49:
		return (int) sys_kill_process((int) arg1);
		break;
	//sys_get_pid
	case 50:
		return (int) sys_get_pid();
		break;
	//sys_print_running_procs: prints process info
	case 51:
		sys_print_running_procs();
		break;
	//sys_set_priority: sets priority to a process
	case 52:
		return (int) sys_set_priority((int) arg1, (int) arg2);
		break;
	//sys_set_state: sets a state to a process
	case 53:
		return (int) sys_set_state((int)arg1, (int) arg2);
		break;
	//sys_create_process_params: to create a process with parameters
	case 54:
		return (uint64_t) sys_create_process_params((int) arg1, (uint64_t) arg2, (uint64_t) arg3, (uint64_t) arg4);
		break;
	case 55:
		return (int) sys_set_stdin((uint64_t) arg1, (int) arg2);
		break;
	case 56:
		return (int) sys_set_stdout((uint64_t) arg1, (int) arg2);
		break;
	case 57:
		return (int) sys_get_stdin();
		break;
	case 58:
		return (int) sys_get_stdout();
		break;
	//sys_create_semaphore: creates a sem and returns sem id
	case 60:
		return (int) sys_create_semaphore((int)arg1, (int) arg2);
		break;
	//sys_sem_close: close sem
	case 61:
		return (int) sys_sem_close((int) arg1);
		break;
	//sys_sem_post: clears sem
	case 62:
		return (int) sys_sem_post((int) arg1);
		break;
	//sys_sem_wait: waits for a sem
	case 63:
		return (int) sys_sem_wait((int) arg1);
		break;
	//sys_print_sems: prints openned semaphores
	case 64:
		sys_print_sems();
		break;
	case 75:
		return (int)sys_open_pipe((int) arg1);
		break;
	case 76:
		sys_close_pipe((int) arg1);
		break;
	case 77:
		return (int) sys_write_pipe((int) arg1, (char *) arg2,(int) arg3);
		break;
	case 78:
		return (int) sys_read_pipe((int) arg1, (char *) arg2, (int) arg3);
		break;
	case 79:
		sys_print_pipe();
		break;

	case 120:
		ncPrintDec(arg1);
		ncNewLine();
		ncPrintDec(arg2);
		ncNewLine();
		ncPrintDec(arg3);
		ncNewLine();
		ncPrintDec(arg4);
		break;
		
	}





	return 0;
}

//SYS_CALL 0
void sys_read_function(char *buffer, int count)
{

	int i = 0;
	char letter;
	while (i < count)
	{
		letter = getChar();
		if (letter != 0)
		{
			*(buffer + i) = letter;
			i++;
		}
		else
		{
			//If there is nothing on the buffer we stop until we get an interruption.
			//haltFunction();
		}
	}
}

//SYS_CALL 1
void sys_write(char *buffer, int count)
{
	ncPrint(buffer);
}

//SYS_CALL 2
void sys_put_char(char c)
{
	if (c != 0)
		ncPrintChar(c);
}

//SYS_CALL 3
void sys_print_action(int action){
	switch(action){
		//If its an enter.
		case 0:
			//prints a new line
			ncNewLine();
			break;
		//If its a backsapce
		case 1:
			ncBackspace();
			break;
	}
}

//SYS_CALL 4
void sys_write_number(int number, int option){
	switch(option){
		case 0:
			ncPrintDec(number);
			break;
		case 1:
			ncPrintHex(number);
			break;

	}
}

//SYS_CALL 5
void sys_change_sound(int option, uint16_t frequency){
	switch (option)
	{
	case 0:
		turnOffBeep();
		break;
	case 1:
		turnOnBeep(frequency);
		break;
	}

}

//SYS_CALL 6
void sys_put_char_fixed(uint16_t x, uint16_t y, uint8_t character){
	if(character!=0){
		drawCharAt(x,y,character);
	}
}

//SYS_CALL 10
uint64_t sys_get_ticks()
{
	return getTicks();
}

//SYS_CALL 11
char sys_get_last_in()
{
	return getLastInput();
}

//SYS_CALL 12
uint8_t sys_get_clock_info(int option){
	switch(option){
		case 0:
			return readSeconds();
		case 1:
			return readMinutes();
		case 2:
			return readHours();	
	}

	return 0;

}

//Strucure used for sys_scr
#pragma pack(push)
#pragma pack(1)
typedef struct Rect
{
	uint16_t x;
	uint16_t y;
	uint16_t base;
	uint16_t height;
	uint8_t blue;
	uint8_t green;
	uint8_t red;
} Rect;
#pragma pack(push)

//SYSCALL 20 for everything screen based
void sys_screen(uint64_t option, uint64_t arg1, uint64_t arg2)
{
	switch (option)
	{
	case 0:
		ncClear();
		break;
	case 1:
		getScreenSize((uint16_t *)arg1, (uint16_t *)arg2);
	case 2:
		drawRectangle(
			coordinate(((Rect *)arg1)->x, ((Rect *)arg1)->y), ((Rect *)arg1)->base, ((Rect *)arg1)->height,
			toColour(((Rect *)arg1)->blue, ((Rect *)arg1)->green, ((Rect *)arg1)->red));
		break;
	}
}

//SYSCALL 45 get memory
//TODO
uint64_t sys_mem_get(long size){
	return (uint64_t) giveMeMemory(size);
}

//SYSCALL 46 free memory
int sys_mem_free(uint64_t chunk){
	return (int)unGiveMeMemory((void *)chunk);
}
//SYSCALL 47 creates a new process
uint64_t sys_create_process(int priority, uint64_t process){
	return (uint64_t) create_process(priority, process);
}
//SYSCALL 48 runs a process
int sys_run_process(uint64_t process, int state){
	return run_process((processInfo) process, state);
}
//SYSCALL 49 kills a running process
int sys_kill_process(int pid){
	return kill_process(pid);
}

//SYSCALL 50 get pid
int sys_get_pid(){
	return get_current_pid();
}
//SYSCALL 51 prints process information
void sys_print_running_procs(){
	print_running_procs();
}
//SYSCALL 52 set priority to a process
int sys_set_priority(int pid, int priority){
	return (int) set_priority(pid, priority);
}

//SYSCALL 53 to set state
int sys_set_state(int pid, int state){
	return (int) set_state(pid, state);
}

//SYSCALL 54 to create a process with parameters
uint64_t sys_create_process_params(int priority, uint64_t rip, uint64_t arg1, uint64_t arg2){
	return (uint64_t) create_process_with_args(priority, rip ,arg1 , arg2);
}

int sys_set_stdin(uint64_t process, int fd){
	int pid = ((processInfo) process) -> pid;
	//ncPrint(pid);
	return set_std_in(pid, fd);
}

int sys_set_stdout(uint64_t process, int fd){
	int pid = ((processInfo) process) -> pid;
	return set_std_out(pid, fd);
}

int sys_get_stdin(){
	return get_stdin();
}

int sys_get_stdout(){
	return get_stdout();
}

//SYSCALL 60 creates a semaphore
int sys_create_semaphore(int name, int state){
	return (int) s_open(name, state);
}

//SYSCALL 61 close an existing semaphore, if does not exist it returns -1
int sys_sem_close(int sid){
	return (int) s_close(sid);
}

//SYSCALL 62 sem post
int sys_sem_post(int sid){
	return (int) s_post(sid);
}

//SYSCALL 63 sem wait
int sys_sem_wait(int sid){
	return (int) s_wait(sid);
}

//SYSCALL 64 print sems
void sys_print_sems(){
	print_sems();
}

//SYSCALL 75 opens a pipe if it doesnt exist it creates one
int sys_open_pipe(int fd){
	return (int) pipe_open(fd);
}
//SYSCALL 76 closes a pipe
void sys_close_pipe(int fd){
	pipe_close(fd);
}

//SYSCALL 77 writes a pipe
int sys_write_pipe(int fd, char * buffer, int size){
	return pipe_write(fd, buffer, size);
}

//SYSCALL 78 reads from a pipe
int sys_read_pipe(int fd, char * buffer, int size){
	return pipe_read(fd, buffer, size);
}

//SYSCALL 79 prints from a pipe
void sys_print_pipe(){
	print_pipes();
}

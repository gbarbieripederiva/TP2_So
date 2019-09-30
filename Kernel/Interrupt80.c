#include <interrupt80.h>
#include <interruptActions.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <keyboard.h>
#include <interrupts.h>
#include <drivers/videoDrivers.h>
#include <lib.h>

//Software interrupt used for interaction between user and kernel space
//order of registers in standard rdi -> call number,rsi -> arg1 ,rdx -> arg2 ,rcx -> arg3
uint64_t interruptAction80Dispatcher(uint64_t callNumber, uint64_t arg1, uint64_t arg2, uint64_t arg3)
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
#define SIZE 100000
#define NULL 0
uint64_t sys_mem_get(long size){
	static char memChunk[SIZE]; 
    static char * currentPtr = memChunk;
    static long used = 0;

    if(used + size > SIZE){
        return NULL;
    }
	char * retVal = currentPtr;
	currentPtr += size;
	used += size;
	return retVal;
}
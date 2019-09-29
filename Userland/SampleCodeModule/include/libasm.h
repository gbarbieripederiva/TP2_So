#include <stdint.h>
#ifndef __LIBASMUSERLAND___
#define __LIBASMUSERLAND__

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

#endif
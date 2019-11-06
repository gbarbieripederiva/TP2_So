#include "include/libasm.h"
#include "include/lib.h"
#include <stdint.h>

//-----------Functions that involve an interaction with the kernel aka int 80h------------
void read(char *buffer, int count)
{
   sys_read_call((uint64_t)buffer, (uint64_t)count);
}

uint64_t getTicks()
{
   return sys_get_ticks_call();
}
void write_fd(int fd, char * buffer){
   if(fd == -1){
      sys_write_call((uint64_t)buffer, (uint64_t)strlength(buffer));
   }
   else{
      sys_write_pipe(fd, buffer, strlength(buffer) + 1);
   }
}

void print(char *buffer)
{
   sys_write_call((uint64_t)buffer, (uint64_t)strlength(buffer));
}

void putChar(char c)
{
   sys_put_char_call((uint64_t)c);
}

/*
   Prints a in screen one of the following commands:
   -An enter o newline if action = 0
   -A backspace(erase a char) if action = 1
 */
void printAction(int action){
   sys_print_action_call((uint64_t)action);
}

//Get last input from the buffer and consume it
char getLastInput()
{
   return sys_get_last_in_call();
}

/* Prints a number on based on the option:
   -0 for decimal
   -1 for hexadecimal
*/
void printDec(int number){
   sys_print_number_call((uint64_t)number,0);
}

/*
   Gets the info of the clock based on the option:
   -Seconds 0
   -Minutes 1
   -Hours 2
 */
int getClockInfo(int option){
   return (int) sys_get_clock_call((uint64_t)option);
}


/*
   Functions to play a sound and stop it.
 */
void playSound(uint16_t freq){
    sys_change_sound_call(1,(uint64_t)freq);
}

void stopSound(){
   sys_change_sound_call(0,0);
}


/*
   Plays a frequency for certain ammount off ticks.
 */
void playFrequency(uint16_t freq, uint64_t ticks){

   sys_change_sound_call(1,(uint64_t)freq);
   uint64_t endTicks = getTicks() + ticks;
   while(endTicks > getTicks()){}//Pause for
   sys_change_sound_call(0,0);

}

/*
   Puts a char in a fixed position determined by x and y.
 */

void putCharFixed(uint16_t x, uint16_t y, uint8_t character){
   sys_put_char_fixed_call((uint64_t)x,(uint64_t)y,(uint64_t)character);
}


//Function to interact with the screen.

void getScreenSize(uint16_t *width, uint16_t *height)
{
   sys_screen(1, (uint64_t)width, (uint64_t)height);
}
void clearScreen()
{
   sys_screen(0, 0, 0);
}
void drawRectangle(Rectangle *rect)
{
   sys_screen(2, (uint64_t)rect, 0);
}



//-----------Functions that don't interact with the kernel--------------------------------

int strlength(char *str)
{
   int i = 0;
   while (str[i] != 0)
   {
      i++;
   }
   return i;
}

int strcmp(const char *str1, const char *str2)
{

   int i = 0;
   while (str1[i] != 0 && str2[i] != 0)
   {
      if (str1[i] != str2[i])
         return 0;
      i++;
   }
   //If one of them still has characters.
   if (str1[i] != str2[i])
      return 0;

   return 1;
}
void strncopy(char *source, char *dest, int size)
{
   int i = 0;
   while (i < size && source[i] != 0)
   {
      dest[i] = source[i];
      i++;
   }
   dest[i] = 0;
}

//Extracted from library
char* intToString(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
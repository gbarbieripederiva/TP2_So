#ifndef __LIB_USERLAND__
#define __LIB_USERLAND__

#include <stdint.h>

#define EMPTY_BUFFER 0

//---------------Functions that involve an interaction with the kernel aka int 80h----------
uint64_t getTicks();
void print(char *buffer);
void putChar(char c);
char getLastInput(); //returns EMPTY_BUFFER if buffer is empty
void read(char *buffer, int count);
void printAction(int action);
void printDec(int number);
int getClockInfo(int option);   //0 for seconds, 1 for minutes, 2 for hours
void playSound(uint16_t freq);
void stopSound();
void playFrequency(uint16_t freq, uint64_t ticks);
void putCharFixed(uint16_t x, uint16_t y, uint8_t character);
void write_fd(int fd, char *buffer);


#pragma pack(push)
#pragma pack(1)
typedef struct rectangle
{
	uint16_t x;
	uint16_t y;
	uint16_t base;
	uint16_t height;
	uint8_t blue;
	uint8_t green;
	uint8_t red;
} Rectangle;
#pragma pack(push)

void getScreenSize(uint16_t *width, uint16_t *height);
void clearScreen();
void drawRectangle(Rectangle *rect);

//-----------Functions that don't interact with the kernel--------------------------
int strlength(char *str);
int strcmp(const char *str1, const char *str2);
void strncopy(char *source, char *dest, int size);
#endif
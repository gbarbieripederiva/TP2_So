#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char *string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
void ncPrintAsTerminal(const char *str);
void ncPrintCharWithColor(char character, uint8_t color);
void ncPrintWithColor(const char *string, uint8_t colors);
void ncPrintAsTerminalWithColor(const char *str, uint8_t colors);
void ncMoveUp();
void ncBackspace();
void ncNewLine();

void simulDialTerminal(int ticks);
#endif
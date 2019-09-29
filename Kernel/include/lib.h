#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);

char *cpuVendor(char *result);

void turnOnBeep(uint16_t frequency);
void turnOffBeep();

uint8_t readCMOS(uint8_t reg);
uint8_t readSeconds();
uint8_t readMinutes();
uint8_t readHours();

void rebootASM();

#endif
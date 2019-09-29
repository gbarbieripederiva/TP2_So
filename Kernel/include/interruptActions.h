#ifndef __INTERRUPTACTIONS__
#define __INTERRUPTACTIONS__
#include <stdint.h>

// what the interrupt 1(int 20h) does
void interruptAction1();
// what the interrupt 2(int 21h) does
void interruptAction2();

// get the amount of ticks since start
// 1tick ~= 1/18 of a second
unsigned long getTicks();

// asm code used for the action2 interrupt
uint8_t interrupt_action_2_asm();
//print register data
//rax:1,rbx:2,...
void printRegister(uint8_t reg, uint64_t regData);
//print exception data
void printException(uint8_t exc);
#endif
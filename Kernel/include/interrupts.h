#ifndef __INTERRUPTS__ASM__
#define __INTERRUPTS__ASM__
#include <stdint.h>

#define helloWorld 1

// disable interrupts
void _cli();
// enable interrupts
void _sti();

// change master pic mask
// disables the bits in 1
// example: 0xFE enables only irq0(timer interrupt)
void picMasterMask(uint8_t mask);
// same but for slave pic
void picSlaveMask(uint8_t mask);

// timer interrupt
void interrupt1();
// keyboard interrupt
void interrupt2();
// int 80h
void interrupt80();

//Halt the machine
void haltFunction();

//exception 0, division by 0
void exception0();
//exception 6, invalid opcode
void exception6();

#endif
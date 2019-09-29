#include <interruptActions.h>
#include <interrupts.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <keyMap.h>
#include <keyboard.h>
#include <lib.h>

static unsigned long ticks = 0;
// called every 55ms. Increases the ticks counter
void interruptAction1()
{
	ticks++;
}

// returns the amount of ticks
unsigned long getTicks()
{
	return ticks;
}

// called when there is an 21h interrupt
// mostly used to get keyboard interrupts
void interruptAction2()
{
	uint8_t c = interrupt_action_2_asm(); //Get key
	keyboard_handler(c);
}

void printRegister(uint8_t reg, uint64_t regData)
{
	// get which register
	switch (reg)
	{
	case 0:
		ncPrint("RSI:");
		break;
	case 1:
		ncPrint("RAX:");
		break;
	case 2:
		ncPrint("RBX:");
		break;
	case 3:
		ncPrint("RCX:");
		break;
	case 4:
		ncPrint("RDX:");
		break;
	case 5:
		ncPrint("RSP:");
		break;
	case 6:
		ncPrint("RBP:");
		break;
	}
	//print register
	ncPrintHex(regData);
	ncNewline();
}

void printException(uint8_t exc)
{
	// print exception info
	switch (exc)
	{
	case 0:
		ncPrint("Excepcion 0:division por cero");
		break;
	case 6:
		ncPrint("Excepcion 6:opcode invalido");
		break;
	}
	ncNewline();
}
//loader in loader asm
void loader();
void reboot()
{
	while (getChar() != 0)
	{
		;
	}
	ncNewline();
	ncPrint("Press any key to exit");
	while (1)
	{
		if (getChar() != 0)
		{
			rebootASM();
		}
	}
	rebootASM();
}
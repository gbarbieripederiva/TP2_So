#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <interrupt80.h>
#include <interrupts.h>
#include <drivers/videoDrivers.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <schedulerTest.h>
#include <semaphore.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void *getStackBase()
{
	return (void *)((uint64_t)&endOfKernel + PageSize * 8 //The size of the stack itself, 32KiB
					- sizeof(uint64_t)					  //Begin at the top of the stack
	);
}

void *initializeKernelBinary()
{

	void *moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	initializeMemory();
	initializeConsole();
	init_processes();
	init_sched();
	init_sems();

	


	


	return getStackBase();
}

int main()
{
	_cli();
	loadIDT();
	//Entering sampleCodeModuleAddress in userland
	uint64_t sampleInfo = create_process(1, (uint64_t)((EntryPoint)sampleCodeModuleAddress));
	run_process(sampleInfo, READY);	
	_sti();
	



	ncPrint("[Finished]");
	return 0;
}

/*
	processInfo test1Aux;
	test1Aux = create_process("test1", 1, (uint64_t)test1);
	run_process(test1Aux, READY);
	processInfo test0Aux;
	test0Aux = create_process("test0", 1, (uint64_t)test0);
	run_process(test0Aux, READY);*/
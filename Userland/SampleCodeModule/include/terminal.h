#ifndef _TERMINAL_USERLAND_
#define _TERMINAL_USERLAND_

#define BACKSPACE 0x08
#define ENTER 0x0A

//-------------------Main functions---------------------------------------------------

void terminal();
int readNewInput();
void handleCommand();


//-------------------Auxiliary Functions-----------------------------------------------
void printNewLineOfTerminal();
void cleanBuffer();
void fillCommandList();
void fillCommand(char * desc, void (*cmdptr)(void));
void waitFor(uint64_t seconds);

//--------------------Command functions-------------------------------------------------
void helpCommand();
void clearCommand();
void timeCommand();
void snakeCommand();
void testDivisionBy0Command();
void testIvalidOpCodeCommand();
void memCommand();
void pipeCommand();
void psCommand();
void loopCommand(uint64_t seconds);
void killCommand(uint64_t pid);
void niceCommand(uint64_t pid, uint64_t priority);
void blockCommand(uint64_t pid);
void unblockCommand(uint64_t pid);
void catCommand(uint64_t string);
void wcCommand(uint64_t string);
void filterCommand(uint64_t string);
void semCommand();


//--------------------Command definitions------------------------------------------------
#define NICE 0
#define PIPECHECK 1
#define CLEAR 2
#define TIME 3
#define HELP 4
#define SNAKE 5
#define TEST_DIVISION_BY_0 6
#define TEST_INVALID_OPCODE 7
#define MEM 8
#define PS 9
#define LOOP 10
#define KILL 11
#define BLOCK 12
#define UNBLOCK 13
#define CAT 14
#define WC 15
#define FILTER 16
#define SEM 17
#define PHYLO 18
#define BACKGROUND 19
#define PIPE 20
#define SLEEP 21





//-------------------Other definitions---------------------------------------------------
static const char * terminalSymbol = "$> ";


#endif
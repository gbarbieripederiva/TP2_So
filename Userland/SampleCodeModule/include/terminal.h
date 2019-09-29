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


//--------------------Command definitions------------------------------------------------
static const char * helpDef = "help";
static const char * snakeDef = "snake";
static const char * clearDef = "clear";
static const char * timeDef = "time";
static const char * testDivisonBy0Def = "testDivisionBy0";
static const char * testInvalidOpCodeDef = "testInvalidOpcode";




//-------------------Other definitions---------------------------------------------------
static const char * terminalSymbol = "$> ";


#endif
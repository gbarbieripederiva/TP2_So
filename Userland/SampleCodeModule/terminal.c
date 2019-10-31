#include "include/lib.h"
#include "include/terminal.h"
#include "include/jueguito.h"

#define BUFFER_SIZE 100
#define MAX_COMDESC 100
#define MAX_COMMANDS 20

//Buffer to store the input from the keyboard.
static char terminalBuffer[BUFFER_SIZE + 1] = {0}; //Non cyclic buffer
static int bufferSize = 0;

//Structure of a command stored in the command List
typedef struct command
{
    char desc[MAX_COMDESC];
    void (*cmdptr)(void);
} command;

//Store all the commands in this array.
static command commandList[MAX_COMMANDS];
static int commandsSize = 0;

//-----------------------------Main functions---------------------------------------------

void terminal()
{

    fillCommandList();

   

    print("Welcome! Please enter a command. Try 'help'");
    printNewLineOfTerminal();

     //int sid = sys_create_semaphore("sem1", 1);
     //sys_sem_wait(sid);
    printDec(sys_set_state(2, 0));

    while (1)
    {
        if (readNewInput())
        {
            handleCommand();
            cleanBuffer();
        }
    }
}

/* Reads from the keyboard if theres new input.
If there is new input(and valid):
    -If its an enter return 1 --> We need to take a command.
    -If its a regular letter just add it to the buffer(if we have size). return 0.
    -If its a backspace erase the letter from the buffer(if there are letters). return 0.
*/
int readNewInput()
{

    char newChar = 0;

    newChar = getLastInput();

    //If there is nothing new or its not a valid character...
    if (newChar == 0)
    {
        return 0;
    }
    //If its just a backsapce
    else if (newChar == BACKSPACE)
    {
        if (bufferSize > 0)
        {
            terminalBuffer[--bufferSize] = 0;
            printAction(1);
        }
        return 0;
    }
    //If its an enter
    else if (newChar == ENTER)
    {
        //Prints a new line.
        printAction(0);
        return 1;
    }
    //If its a regular letter.
    else
    {
        if (bufferSize <= 100)
        {
            terminalBuffer[bufferSize++] = newChar;
            putChar(newChar);
            return 0;
        }
    }

    //Just in case
    return 0;
}

/* Means that the enter key was pressed so we must take action based on 
   the current state of the buffer.
 */
void handleCommand()
{

    //Copy the command into the array. Did this to avoid a bug in which in some cases the buffer
    //represented more chars that it should.
    char potentialCommand[MAX_COMDESC] = {0};
    strncopy(terminalBuffer, potentialCommand, bufferSize);

    for (int i = 0; i < commandsSize; i++)
    {
        if (strcmp(potentialCommand, commandList[i].desc))
        {
            (commandList[i].cmdptr)();

            //After executing the command we print a newLine and exit.
            printNewLineOfTerminal();
            return;
        }
    }

    //If command not found
    print("Not a valid command: ");
    print(potentialCommand);
    printNewLineOfTerminal();
}

//----------------------Possible Commands-------------------------------------------

void helpCommand()
{
    print("You have entered the help command.The following commands are available:");
    for (int i = 0; i < commandsSize; i++)
    {
        printAction(0); //New Line
        print("Command ");
        printDec(i);
        print(": "); //Just for stethic
        print(commandList[i].desc);
    }
}

void clearCommand()
{
    clearScreen();
    print("Screen cleared");
    printAction(0); //New Line
    print("Please enter a command. Try 'help'.");
}

void timeCommand()
{
    print("Time (UTC): ");
    printDec(getClockInfo(2));
    print(":");
    printDec(getClockInfo(1));
    print(":");
    printDec(getClockInfo(0));
}

void snakeCommand()
{
    clearScreen();
    print("Entering snake game.");
    printAction(0);
    print("Press W to turn left, S to turn down, A to turn left, D to turn right.");
    printAction(0);
    print("Survive as long as you can.");
    waitFor(5);
    game();
}

void testDivisionBy0Command()
{
    int a = 4 / 0;
}

void testIvalidOpCodeCommand()
{
    void (*punt)(void) = (void *)0x400000;
    *((uint64_t *)punt) = 0xffffffff;
    (punt)();
}


//----------------------AUXILIARY FUNCTIONS-----------------------------------------
void cleanBuffer()
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        terminalBuffer[i] = 0;
    }
    bufferSize = 0;
}

void printNewLineOfTerminal()
{
    printAction(0);        //Prints an enter.
    print(terminalSymbol); //Prints the symbol of terminal
}

void waitFor(uint64_t seconds)
{
    uint64_t end = getTicks() + (int)(seconds / 0.055); //convertion to ticks
    while (getTicks() < end)
    {
        //Nothing to see here.
    }
}

void fillCommandList()
{
    fillCommand(helpDef, &helpCommand);
    fillCommand(snakeDef, &snakeCommand);
    fillCommand(clearDef, &clearCommand);
    fillCommand(timeDef, &timeCommand);
    fillCommand(testDivisonBy0Def, &testDivisionBy0Command);
    fillCommand(testInvalidOpCodeDef, &testIvalidOpCodeCommand);

}

void fillCommand(char *desc, void (*cmdptr)(void))
{
    command aux;
    strncopy(desc, aux.desc, strlength(desc));
    aux.cmdptr = cmdptr;
    commandList[commandsSize++] = aux;
}

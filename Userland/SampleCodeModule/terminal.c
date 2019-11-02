#include <lib.h>
#include <terminal.h>
#include <jueguito.h>
#include <libasm.h>

#define BUFFER_SIZE 100
#define MAX_COMDESC 100
#define MAX_COMMANDS 20
#define COMMANDS 2
#define SPACE 32
#define MARKS 34

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


//command handler variables
char *commands[COMMANDS] = {"nice","pipe"}; //all command names
int tokenIterator = 0; //used to process multiple tokens
int tokens = 0; //we need token to be visible for extract token function and for the handler

//-----------------------------Main functions---------------------------------------------

void terminal()
{

    fillCommandList();

   

    print("Welcome! Please enter a command. Try 'help'");
    printNewLineOfTerminal();
     int sid = sys_create_semaphore(1 , SEM_LOCKED);
     //sys_sem_wait(sid);
    //printDec(sys_set_state(2, 0));

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

//______________________________________________________________new handler__________________________________________________________________________

//tokens
int tokensCounter(char *string){ //counting all tokens found in the string submitted
    int i = 0; //iterates inside the string
    int tokens = 1; //it starts in the first token
    int insideString = 0; //checks if it is inside a string which doesnt count as a token
    while(string[i] != 0){
        if(string[i] == MARKS){ // if it is a mark, then it means we are starting or ending a string
            insideString = !insideString;
        }
        if(string[i] == SPACE && !insideString){ //counts another token
            tokens++;
        }
        i++;
    }
    return tokens;
}

void extractToken(char *dest, char *string, int tokenNum){ //extracts an specific token
    int insideString = 0;
    int i = 0;
    int count = 0;
    while(count < tokenNum - 1 && string[i] != 0){ //same logic as token counter
        if(string[i] == MARKS){
            insideString = !insideString;
        }
        else if(string[i] == SPACE && !insideString){
            count++;
        }
        i++;
    }
    int j = 0; //copying the token in the destination
    while(string[i] != 0 && string[i] != SPACE){
        dest[j] = string[i];
        j++;
        i++;
    }
    dest[j] = 0;
    tokens --;

}
int power(int base, int exponent){ //we need power to get the string into an integer
    int aux = 1;
    if(exponent < 0){
        return -1;
    }
    else{
        while(exponent > 0){
            aux = aux * base;
            exponent-- ;
        }
    }
    return aux;
}
int isNumber(char i){ //we have to check it is a number, else is not a valid argument
    if( i >= 48 && i <= 57){
        return 1;
    }
    else{
        return 0;
    }
}

int stringToInt(char *string){ //logic
    int i = 0;
    int aux = 0;
    int size = strlength(string);
    while(size > 0){
        if(isNumber(string[i])){ 
            aux += (((int)string[i]) - 48) * power(10, size - 1);
            i++;
            size --;
        }
        else{ //if it is not a number then it is not a valid argument
            print("Not a valid argument");
            printAction(0);
            return -1;
        }
    }
    return aux;

}
void handleToken(char *string, int tokenNum){ //we need to execute the correct function
    char justCommand[MAX_COMDESC] = {0};
    extractToken(justCommand, string, tokenNum); //we want to extract the token we need

    int commandNum = 0;
    while(commandNum < COMMANDS && (strcmp(justCommand, commands[commandNum]) != 0)){ //we look for the command that matches
        commandNum++;
    }


    switch (commandNum) //we execute the command or we tell the user it does not exist
    {
    case 0: ;
        char arg1[64];
        char arg2[64];
        extractToken(arg1, string, tokenIterator + 1);
        extractToken(arg2, string, tokenIterator + 2);
        tokenIterator = tokenIterator + 2;
        niceCommand(stringToInt(arg1), stringToInt(arg2));
        break;
    case 1:
        pipeCommand();
        return;
        break;


    default:
        print("Command not fount");
        printAction(0);
        break;
    }
}

void handleCommand(){
    tokenIterator = 0;
    char potentialCommand[MAX_COMDESC];
    strncopy(terminalBuffer, potentialCommand, bufferSize);
    tokens = tokensCounter(potentialCommand);

    while(tokens != 0){ //consumes all the tokens the potential command has
        tokenIterator++;

        handleToken(potentialCommand, tokenIterator);
    }
}






//______________________________________________________________________________________________________________________________________________________








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

void memCommand()
{
    //TODO
}

void pipeCommand() {
    sys_print_pipe();
}

void psCommand(){
    sys_print_running_procs();
}

void loopCommand(uint64_t seconds){
    while(1){
        waitFor(seconds);
        printAction(0);
        print("Welcome to Barca SO");
        printAction(0);
        print("PID: ");
        printDec(sys_get_pid);
        printAction(0);
    }
}

void killCommand(int pid){
    if(sys_kill_process(pid) != -1){
        printAction(0);
        print("Succesfully killed");
        printAction(0);
    }
    else{
        printAction(0);
        print("No such process running");
        printAction(0);
    }
}

void niceCommand(int pid, int priority){
    if(sys_set_priority(pid, priority) != -1){
        printAction(0);
        print("Succesfully priority set");
        printAction(0);
    }
    else{
        printAction(0);
        print("No such process running");
        printAction(0);
    }
}

void blockCommand(int pid){
      if(sys_set_state(pid, PROC_BLOCK) != -1){
        printAction(0);
        print("Succesfully blocked");
        printAction(0);
    }
    else{
        printAction(0);
        print("No such process running");
        printAction(0);
    }
}

void unblockCommand(int pid){
      if(sys_set_state(pid, PROC_RUNNING) != -1){
        printAction(0);
        print("Succesfully unblocked");
        printAction(0);
    }
    else{
        printAction(0);
        print("No such process running");
        printAction(0);
    }
}

void catCommand(char *string){//dont know if it should be redirected
        printAction(0);
        print(string);
        printAction(0);
}

void wcCommand(char *string){

}

void filterCommand(char *string){

}

void semCommand(){
    sys_print_sems();
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


/*void handleCommand()
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
*/
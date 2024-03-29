#include <lib.h>
#include <terminal.h>
#include <jueguito.h>
#include <libasm.h>
#include <phylo.h>

#define BUFFER_SIZE 100
#define MAX_COMDESC 100
#define MAX_COMMANDS 2
#define COMMANDS 21
#define SPACE 32
#define MARKS 34
#define PIPE_MARK 124
#define M_PIPES 10
#define N_OVERLAP 12345


//Buffer to store the input from the keyboard.
static char terminalBuffer[BUFFER_SIZE + 1] = {0}; //Non cyclic buffer
static int bufferSize = 0;
int fd = 10;
int changeStdin = 0;
int pipes[10] = {-1};
int pipeNum = 0;
int not_same = 25;






//command handler variables
char *commands[COMMANDS] = {0}; //all command names
int tokenIterator = 0; //used to process multiple tokens
int tokens = 0; //we need token to be visible for extract token function and for the handler
int background = 0; //checks if it has to be executed in background
int pipesAmount = 0; //counts the amounts of pipes in a command

//-----------------------------Main functions---------------------------------------------

void terminal()
{

    fillCommandList();
    
    
    print("Welcome! Please enter a command. Try 'help'");
    printNewLineOfTerminal();
    



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
    if(string[i] == MARKS){
        i++;
        while (string[i] != 0 && string[i] != MARKS)
        {
            dest[j] = string[i];
            j++;
            i++;
        }
        
    }
    else{
        while(string[i] != 0 && string[i] != SPACE){
            dest[j] = string[i];
            j++;
            i++;
        }
    }
    dest[j] = 0;
    tokenIterator ++;

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
    while(commandNum < COMMANDS && (strcmp(justCommand, commands[commandNum]) != 1)){ //we look for the command that matches
        commandNum++;
    }


    switch (commandNum) //we execute the command or we tell the user it does not exist
    {
    case NICE: ;
        
        if(tokens >= tokenNum + 2){
            char arg1[64];
            char arg2[64];
            extractToken(arg1, string, tokenNum + 1);
            extractToken(arg2, string, tokenNum + 2);

            if(background == 1 || pipesAmount != 0){
                background = 0;
                uint64_t niceInfo = sys_create_process_params(0, (uint64_t) niceCommand, (uint64_t) stringToInt(arg1), (uint64_t) stringToInt(arg2));
                if(pipesAmount != 0){
                    //TODO PIPE
                }
                sys_run_process(niceInfo, PROC_RUNNING);
            }
            else{
                niceCommand((uint64_t) stringToInt(arg1), (uint64_t) stringToInt(arg2));
            }
        }
        else{
            tokenIterator = tokens + 1; // this is to stop the while iteration
            print("Not enough arguments for command nice");
            printAction(0);
        }
        break;


    case PIPECHECK:
        if(background == 1 || pipesAmount != 0){
            background = 0;
            uint64_t pipeInfo = sys_create_process(0, (uint64_t) pipeCommand);
            if(pipesAmount != 0){
                //TODO
            }
            sys_run_process(pipeInfo, PROC_RUNNING);
        }
        else{
            pipeCommand();}
        break;


    case CLEAR:
        clearCommand();
        break;


    case TIME:
        if(background == 1 || pipesAmount != 0){
            background = 0;
            uint64_t timeInfo = sys_create_process(0, (uint64_t)timeCommand);
            if(pipesAmount != 0){
                //TODO
            }
            sys_run_process(timeInfo, PROC_RUNNING);
        }
        else{
            timeCommand();
        }
        break;


    case HELP:
        if(background == 1 || pipesAmount != 0){
            background = 0;
             uint64_t helpInfo = sys_create_process(0, (uint64_t) helpCommand);
             if(pipesAmount != 0){
                 //TODO
             }
            sys_run_process(helpInfo, PROC_RUNNING);
        }
        else
        {
            helpCommand();
        }
        
       
        break;


    case SNAKE:
        if(background == 1 || pipesAmount != 0){
            background = 0;
            uint64_t snakeInfo = sys_create_process(0, (uint64_t) snakeCommand);
            if(pipesAmount != 0){
                //TODO
            }
            sys_run_process(snakeInfo, PROC_RUNNING);
        }
        else
        {
            snakeCommand();    
        }
        
        break;


    case TEST_DIVISION_BY_0:
        if(background == 1 || pipesAmount != 0){
            background = 0;
            uint64_t testDivInfo = sys_create_process(0, (uint64_t) testDivisionBy0Command);
            if(pipesAmount != 0){

            }
            sys_run_process(testDivInfo, PROC_RUNNING);
        }
        else{
            testDivisionBy0Command();
        }
        break;


    case TEST_INVALID_OPCODE:
        if(background == 1 || pipesAmount != 0){
            background = 0;
            uint64_t testInvalidInfo = sys_create_process(0, (uint64_t) testIvalidOpCodeCommand);
            if(pipesAmount != 0){

            }
            sys_run_process(testInvalidInfo, PROC_RUNNING);
        }

        else
        {
            testIvalidOpCodeCommand();
        }
        
        
        break;


    case MEM:
         if(background == 1 || pipesAmount != 0){
             background = 0;
            uint64_t memInfo = sys_create_process(0, (uint64_t) memCommand);
            if(pipesAmount != 0){
                //TODO
            }
            sys_run_process(memInfo, PROC_RUNNING);
        }
        else{
            memCommand();
        }
        break;


    case PS:
        psCommand();
        break;


    case LOOP: ;
        if(tokens >= tokenNum + 1){
            char arg1[64];
            extractToken(arg1, string, tokenNum + 1);
            if(background == 1 || pipesAmount != 0){
                background = 0;
                uint64_t loopInfo = sys_create_process_params(0, (uint64_t)loopCommand,(uint64_t) stringToInt(arg1), 0);
                if(pipesAmount != 0){
                    //TODO
                }
                sys_run_process(loopInfo, PROC_RUNNING);
            }
            else{
                loopCommand((uint64_t) stringToInt(arg1));
            }
        }
        else{
            tokenIterator = tokens + 1; // this is to stop the while iteration
            print("Not enough arguments for command loop");
            printAction(0);
        }
        break;


    case KILL:
         if(tokens >= tokenNum + 1){
            char arg1[64];
            extractToken(arg1, string, tokenNum + 1);
            if(background == 1 || pipesAmount != 0){
                background = 0;
                uint64_t killInfo = sys_create_process_params(0, (uint64_t) killCommand, (uint64_t)stringToInt(arg1), (uint64_t) 0);
                if(pipesAmount != 0){
                    //TODO
                }
                sys_run_process(killInfo, PROC_RUNNING);
            }
            else{
                killCommand((uint64_t) stringToInt(arg1));
            }
        }
        else{
            tokenIterator = tokens + 1; // this is to stop the while iteration
            print("Not enough arguments for command kill");
            printAction(0);
        }
        break;


    case BLOCK:
         if(tokens >= tokenNum + 1){
            char arg1[64];
            extractToken(arg1, string, tokenNum + 1);
            if(background == 1 || pipesAmount != 0){
                background = 0;
                uint64_t blockInfo = sys_create_process_params(0, (uint64_t)blockCommand, (uint64_t) stringToInt(arg1), 0);
                if(pipesAmount != 0){
                    //TODO
                }
                sys_run_process(blockInfo, PROC_RUNNING);
            }
            else{
                blockCommand((uint64_t) stringToInt(arg1));
            }
        }
        else{
            tokenIterator = tokens + 1; // this is to stop the while iteration
            print("Not enough arguments for command block");
            printAction(0);
        }
        break;


    case UNBLOCK:
          if(tokens >= tokenNum + 1){
            char arg1[64];
            extractToken(arg1, string, tokenNum + 1);
            if(background == 1 || pipesAmount != 0){
                background = 0;
                uint64_t unblockInfo = sys_create_process_params(0, (uint64_t)unblockCommand, (uint64_t) stringToInt(arg1), 0);
                if(pipesAmount != 0){
                    //TODO
                }
                sys_run_process(unblockInfo, PROC_RUNNING);
            }
            else{
                unblockCommand((uint64_t) stringToInt(arg1));
            }
        }
        else{
            tokenIterator = tokens + 1; // this is to stop the while iteration
            print("Not enough arguments for command unblock");
            printAction(0);
        }
        break;



    case CAT:
        if(tokens >= tokenNum + 1){
            char arg1[64];
            extractToken(arg1, string, tokenNum + 1);
            if(background == 1 || pipesAmount != 0){
                background = 0;
                uint64_t catInfo = sys_create_process_params(0, (uint64_t)catCommand , (uint64_t) arg1, 0);
                if(changeStdin == 1){
                    sys_set_stdin(catInfo, pipes[pipeNum]);
                    pipesAmount--;
                    changeStdin = 0;
                    pipeNum ++;
                }
                if(pipesAmount != 0){
                    sys_set_stdout(catInfo ,pipes[pipeNum]);
                }
                
                sys_run_process(catInfo, PROC_RUNNING);
            }
            else{
                
                catCommand((uint64_t) arg1);
            }
        }
        else{
              if(background == 1 || pipesAmount != 0){
                background = 0;
                uint64_t catInfo = sys_create_process_params(0, (uint64_t)catCommand , (uint64_t) 0, 0);
                 if(changeStdin == 1){
                    sys_set_stdin(catInfo, pipes[pipeNum]);
                    pipesAmount--;
                    changeStdin = 0;
                    pipeNum ++;
                }
                if(pipesAmount != 0){
                    sys_set_stdout(catInfo ,pipes[pipeNum]);
                }
               
                sys_run_process(catInfo, PROC_RUNNING);
            }
            else{
            catCommand((uint64_t)0);}
        }
        break;

    case WC:
        if(tokens >= tokenNum + 1){
            
            char arg1[64];
            extractToken(arg1, string, tokenNum + 1);
            if(background == 1 || pipesAmount != 0){
                background = 0;
                uint64_t wcInfo = sys_create_process_params(0, (uint64_t)wcCommand , (uint64_t) arg1, 0);
                if(changeStdin == 1){
                    sys_set_stdin(wcInfo, pipes[pipeNum]);
                    pipesAmount--;
                    changeStdin = 0;
                    pipeNum ++;
                }
                if(pipesAmount != 0){
                    sys_set_stdout(wcInfo ,pipes[pipeNum]);
                }
                
                sys_run_process(wcInfo, PROC_RUNNING);
            }
            else
            {
             
                wcCommand((uint64_t) arg1);    
            }
        }
        else{
                 if(background == 1 || pipesAmount != 0){
                uint64_t wcInfo = sys_create_process_params(0, (uint64_t)wcCommand , (uint64_t) 0, 0);
                if(changeStdin == 1){
                    sys_set_stdin(wcInfo, pipes[pipeNum]);
                    changeStdin = 0;
                    pipeNum ++;
                }
                if(pipesAmount != 0){
                    sys_set_stdout(wcInfo ,pipes[pipeNum]);
                }
             
                sys_run_process(wcInfo, PROC_RUNNING);
            }
            else{
                wcCommand((uint64_t) 0);}
        }
        break;


    case FILTER:
         if(tokens >= tokenNum + 1){
            background = 0;
            char arg1[64];
            extractToken(arg1, string, tokenNum + 1);
           
            if(background == 1 || pipesAmount != 0){
                uint64_t filterInfo = sys_create_process_params(0, (uint64_t)filterCommand , (uint64_t) arg1, 0);
                  if(changeStdin == 1){
                    sys_set_stdin(filterInfo, pipes[pipeNum]);
                    pipesAmount--;
                    changeStdin = 0;
                    pipeNum ++;
                }
                 if(pipesAmount != 0){
                     
                    sys_set_stdout(filterInfo ,pipes[pipeNum]);
                }
              
                sys_run_process(filterInfo, PROC_RUNNING);
            }
            else
            {
                filterCommand((uint64_t) arg1);    
            }
            
        }
        else{
               if(background == 1 || pipesAmount != 0){
                uint64_t filterInfo = sys_create_process_params(0, (uint64_t)filterCommand , (uint64_t)0, 0);
                 if(changeStdin == 1){
                    sys_set_stdin(filterInfo, pipes[pipeNum]);
                    pipesAmount--;
                    changeStdin = 0;
                    pipeNum ++;
                }
                 if(pipesAmount != 0){
                    sys_set_stdout(filterInfo ,pipes[pipeNum]);
                }
                
                sys_run_process(filterInfo, PROC_RUNNING);
            }
            else{
                filterCommand((uint64_t) 0);}
        }
        break;


    case SEM:
        if(background == 1 || pipesAmount != 0){
            background = 0;
            uint64_t semInfo = sys_create_process(0, (uint64_t) semCommand);
            if(pipesAmount != 0){
                //TODO
            }
            sys_run_process(semInfo, PROC_RUNNING);
        }
        else
        {
            semCommand();    
        }
        
        break;

    case PHYLO:
        if(tokens >= tokenNum + 1){
            char arg1[64];
            extractToken(arg1, string, tokenNum + 1);
            phyloCommand((uint64_t) stringToInt(arg1));

        }
        else
        {
            phyloCommand(5);
        }
        
        
        break;
    
    case BACKGROUND: //it has to be called with "& command1 parameters"
        background = 1;
        break;
    
    case PIPE:
        changeStdin = 1;
        break;




    default:
        tokenIterator = tokens + 1; // this is to stop the while iteration
        print("Command not found");
        printAction(0);
        break;
    }
    if(background != 1){
        printNewLineOfTerminal();
    }
}

int pipesCount(char * string){
    int insideString = 0;
    int counter = 0;
    int i = 0;
    while(string[i] != 0){
        if(string[i] == MARKS){
            insideString = !insideString;
        }
        if(string[i] == PIPE_MARK){
            counter++;
        }
        i++;
    }
    return counter;
}

void handleCommand(){
    background = 0;
    pipeNum = 0;
    tokenIterator = 1;
    changeStdin = 0;
    char potentialCommand[MAX_COMDESC];
    strncopy(terminalBuffer, potentialCommand, bufferSize);
    pipesAmount = pipesCount(potentialCommand);
    tokens = tokensCounter(potentialCommand);
    int i = 0;
    while(i < M_PIPES && i != pipesAmount){
        if(i == M_PIPES){
            print("More pipes than permited");
            return;
        }
        pipes[i] = sys_open_pipe(not_same + N_OVERLAP);
        not_same ++;
        i++;
    }
    while(tokens + 1 > tokenIterator){ //consumes all the tokens the potential command has
        handleToken(potentialCommand, tokenIterator);
    }
}






//______________________________________________________________________________________________________________________________________________________








//----------------------Possible Commands-------------------------------------------

void helpCommand()
{
    print("You have entered the help command.The following commands are available:");
    for (int i = 0; i < COMMANDS; i++)
    {
        printAction(0); //New Line
        print("Command ");
        printDec(i);
        print(": "); //Just for stethic
        print(commands[i]);
        if(i == LOOP){
            print("      (Be careful if it is not called in background it blocks the terminal)");
        }
        else if(i == BACKGROUND){
            print("      (format: \"& command parameters\")");
        }
    }
    printAction(0);
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
    sys_print_mem();
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
        printDec(sys_get_pid());
        printAction(0);
    }
}

void killCommand(uint64_t pid){
    if(pid != 0 && pid != 2 &&sys_kill_process((int) pid) != -1){
        printAction(0);
        print("Succesfully killed");
        printAction(0);
    }
    else if(pid == 0 || pid == 2){
        print("Error, it is not permitted to kill process halt or terminal.");
    }
    else{
        printAction(0);
        print("No such process running");
        printAction(0);
    }
}

void niceCommand(uint64_t pid, uint64_t priority){
    if(sys_set_priority((int) pid, (int)priority) != -1){
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

void blockCommand(uint64_t pid){
      if(sys_set_state((int) pid, PROC_BLOCK) != -1){
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

void unblockCommand(uint64_t pid){
      if(sys_set_state((int) pid, PROC_RUNNING) != -1){
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



void catCommand(uint64_t string){//dont know if it should be redirected
        int stdin = sys_get_stdin();
        char buff[100] = {0};
        int stdout = sys_get_stdout();
        if(string != 0){
        write_fd(stdout, (char *) string); }
        sys_read_pipe(stdin, buff, 100);
        write_fd(stdout, buff);
       
        
}


void wcCommand(uint64_t string){
        int stdin = sys_get_stdin();
        char buff[100] = {0};
        if(string != 0){
        sys_write_pipe(stdin, (char *)string, strlength((char *)string) + 1);
        }
        sys_read_pipe(stdin, buff, 100);

        int strlen = strlength(buff);
        int j = 0;
        int counter = 1;
        while(j < strlen){
            if(buff[j] == 10){
                counter++;
            }
            j++;
        }
        char * res;
        char aux[100];
        res = intToString(counter, aux, 10);

        int stdout = sys_get_stdout();
        write_fd(stdout, res);
}

int isVowel(char a){
    if(a == 65 || a == 69 || a == 73 || a == 79 || a == 85 || a == 97 || a == 101|| a == 105|| a == 111 || a == 117){
        return 1;
    }
    return 0;
}

void filterCommand(uint64_t string){
        int stdin = sys_get_stdin();
        char buff[100] = {0};
        if(string != 0){
            sys_write_pipe(stdin, (char *)string, strlength((char *)string) + 1);}
        sys_read_pipe(stdin, buff, 100);
        char res[100] = {0};
        int h = 0;
        int j = 0;
        while(j < 100 && buff[j] != 0){
            if(!(isVowel(buff[j]))){
                res[h] = buff[j];
                h++;
            }
            j++;
        }


        int stdout = sys_get_stdout();
        write_fd(stdout, res);
}

void semCommand(){
    sys_print_sems();
}

void phyloCommand(int h){
    print("Press 'a' to add a phylosopher");
    printAction(0);
    print("Press 'd' to delete a phylosopher");
    printAction(0);
    print("Press 'q' to quit program");
    printAction(0);
     start_phylo(h);
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
    commands[0] = "nice";
    commands[1] = "pipe";
    commands[2] = "clear";
    commands[3] = "time";
    commands[4] = "help";
    commands[5] = "snake";
    commands[6] = "testDivisionBy0";
    commands[7] = "testInvalidOpcode";
    commands[8] = "mem";
    commands[9] = "ps";
    commands[10] = "loop";
    commands[11] = "kill";
    commands[12] = "block";
    commands[13] = "unblock";
    commands[14] = "cat";
    commands[15] = "wc";
    commands[16] = "filter";
    commands[17] = "sem";
    commands[18] = "phylo";
    commands[19] = "&";
    commands[20] = "|";

}


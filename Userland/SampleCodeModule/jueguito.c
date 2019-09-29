#include <stdint.h>
#include <lib.h>
#include <jueguito.h>

#define HEAD_SIZE 16       //sidelength of every piece of the snake
#define STARTING_SIZE 10    //how long the snake should be initially
#define MAX_WIDTH 64       //unfortunately, we do not having dynamic memory allocation, so we must have the game-size at compile time
#define MAX_HEIGHT 46
#define INPUT_BUFFER_SIZE 3
#define SCREEN_WITDH 1024    //van hardcodeadas por no haber memoria dinamica
#define SCREEN_HEIGHT 768
#define PIXELS_PER_CHAR 8    //importado de videoDrivers

typedef struct cartesian{
    uint16_t x;
    uint16_t y;
} Cartesian;

static enum Sounds {FIRST = 0, SECOND, NONE};
typedef enum Sounds Sounds;

static enum Direction {UP = 0, RIGHT, DOWN, LEFT, EMPTY};
typedef enum Direction Direction;

//static uint16_t width;
//static uint16_t height;
static uint64_t ticksAtUpdate;   //1 every 55ms
static uint64_t currentTicks;
static uint64_t ticksAtEnlargement;
static Direction currentDirection;
static Cartesian head;
static Cartesian tail;
static Direction snake[MAX_WIDTH][MAX_HEIGHT];  //each position saves the direction in which the next snake body part is at. This is what peak efficiency looks like
static uint8_t endGame;
static Rectangle body;
static Rectangle noBody;
static Rectangle topBar;
static uint8_t userInput;
static Direction inputBuffer[INPUT_BUFFER_SIZE];         //guarda hasta 3 para poder combinar teclas muy rapido
static uint8_t inputBufferFirst;                        //inputBuffer es una queue
static uint8_t inputBufferLast;
static uint8_t inputBufferLoad;
static uint8_t currentLevel;
static uint64_t score;
static uint64_t ticksActuales;
static uint64_t ticksAtStart;
static uint64_t ticksAtSecondUpdate;
static uint8_t seconds;
static uint8_t minutes;
static uint8_t hours;
static uint8_t *endGameMessage;
static Sounds sound;

static void initializeMatrix(){
    for(int i = 0; i < MAX_WIDTH; i++){
        for(int j = 0; j < MAX_HEIGHT; j++){
            snake[i][j] = EMPTY;
        }
    }
}

static void moveTail(){
    noBody.x = tail.x * HEAD_SIZE;
    noBody.y = (tail.y + 2) * HEAD_SIZE; // correccion por dejar el tope de pantalla libre
    drawRectangle(&noBody);
    Direction old = snake[tail.x][tail.y];
    snake[tail.x][tail.y] = EMPTY;
    switch (old){
        case UP: 
            tail.y --;
            break;
        case DOWN: 
            tail.y ++;
            break;
        case RIGHT:
            tail.x ++;
            break;
        case LEFT:
            tail.x --;
            break;
        case EMPTY:    //same as below
        default:    ;  //how did you end up here?
    }
}

static void moveHead(){
    snake[head.x][head.y] = currentDirection;
    switch(currentDirection){
        case UP:
            if(head.y == 0 || snake[head.x][head.y - 1] != EMPTY){
                endGame = 1;
            }
            else{
                head.y --;
            }
            break;
        case DOWN:
            if(head.y == MAX_HEIGHT - 1 || snake[head.x][head.y + 1] != EMPTY){
                endGame = 1;
            }
            else{
                head.y ++;
            }
            break;
        case LEFT:
            if(head.x == 0 || snake[head.x - 1][head.y] != EMPTY){
                endGame = 1;
            }
            else{
                head.x --;
            }
            break;
        case RIGHT:
            if(head.x == MAX_WIDTH - 1 || snake[head.x + 1][head.y] != EMPTY ){
                endGame = 1;
            }
            else{
                head.x ++;
            }
            break;
        case EMPTY:                        //No, really, this shouldn't happen
        default:   ;                        //here again?
    }
    if(! endGame){
        body.x = head.x * HEAD_SIZE;
        body.y = (head.y + 2) * HEAD_SIZE; //correccion por tope de pantalla
        drawRectangle(&body);
    }
}

static void moveSnake(){
    moveTail();
    moveHead();
}

static void initializeRectangles(){
    body.base = noBody.base = HEAD_SIZE;
    body.height = noBody.height = HEAD_SIZE;
    body.blue = 0; body.green = 200; body.red = 0; //Not even Eric Clapton's was this white
    noBody.blue = noBody.green = noBody.red = 0; //
    topBar.height = HEAD_SIZE;
    topBar.base = SCREEN_WITDH;
    topBar.x = 0;
    topBar.y = 1 * HEAD_SIZE;
    topBar.blue = topBar.green = topBar.red = 255;
}

static void cleanBuffer(){
    while(getLastInput() != 0);
}

static void startTimer(){
    putCharFixed(0, 0, '0');                        //first hour digit
    putCharFixed(PIXELS_PER_CHAR, 0, '0');          //second hour digit
    putCharFixed(2 * PIXELS_PER_CHAR, 0,':');
    putCharFixed(3 * PIXELS_PER_CHAR, 0, '0');      //first minute digit
    putCharFixed(4 * PIXELS_PER_CHAR, 0, '0');      //second minute digit
    putCharFixed(5 * PIXELS_PER_CHAR, 0, ':');
    putCharFixed(6 * PIXELS_PER_CHAR, 0, '0');      //first second digit
    putCharFixed(7 * PIXELS_PER_CHAR, 0, '0');      //second second digit
}

static void updateTime(){
    ticksActuales = getTicks();
    if(ticksActuales - ticksAtSecondUpdate > 17){
        ticksAtSecondUpdate = ticksActuales;
        if(seconds == 59){
            seconds = 0;
            if(minutes == 59){
                minutes = 0;
                if(hours == 99){
                    endGame = 1;
                    endGameMessage = "You have exceeded the time limit!";
                }
                else{
                    hours++;
                }
                if(hours % 10 == 0){
                    putCharFixed(0, 0, '0' + hours / 10);                        //first hour digit
                }
                putCharFixed(PIXELS_PER_CHAR, 0, '0' + hours % 10);          //second hour digit
            }
            else{
                minutes++;
            }
            if(minutes % 10 == 0){
                putCharFixed(3 * PIXELS_PER_CHAR, 0, '0' + minutes / 10);      //first minute digit
            }
            putCharFixed(4 * PIXELS_PER_CHAR, 0, '0' + minutes % 10);      //second minute digit
        }
        else{
            seconds++;
        }
        if(seconds % 10 == 0){
            putCharFixed(6 * PIXELS_PER_CHAR, 0, '0' + seconds / 10);      //first second digit
        }
        putCharFixed(7 * PIXELS_PER_CHAR, 0, '0' + seconds % 10);      //second second digit
    }
}

static void initializeGame(){
    initializeRectangles();
    initializeMatrix();
    endGame = 0;
    inputBufferFirst = 0;
    inputBufferLast = -1;
    inputBufferLoad = 0;
    currentLevel = 8;
    head.x = tail.x = MAX_WIDTH / 2;
    head.y = tail.y = MAX_HEIGHT / 2;
    body.x = head.x * HEAD_SIZE;
    body.y = (head.y + 2) * HEAD_SIZE; //correeccion blanco pantalla
    currentDirection = RIGHT;
    clearScreen();
    seconds = minutes = hours = 0;
    startTimer();
    drawRectangle(&topBar);
    cleanBuffer();
    drawRectangle(&body);
    for(int i = 0; i < STARTING_SIZE - 1; i++){
        moveHead();
    }
    userInput = EMPTY_BUFFER;
    score = 0;
    ticksAtSecondUpdate = ticksAtStart = ticksActuales = ticksAtEnlargement = getTicks();
    endGameMessage = "Game finished!";
    sound = NONE;
}

static void enqueueToBuffer(Direction d){
    if(inputBufferLoad == INPUT_BUFFER_SIZE){
        return;
    }
    inputBufferLast++;
    if(inputBufferLast == INPUT_BUFFER_SIZE){
        inputBufferLast = 0;
    }
    inputBuffer[inputBufferLast] = d;
    inputBufferLoad++;
}

static Direction dequeueFromBuffer(){
    Direction retVal = inputBuffer[inputBufferFirst];
    inputBufferFirst++;
    if(inputBufferFirst == INPUT_BUFFER_SIZE){
        inputBufferFirst = 0;
    }
    inputBufferLoad--;
    return retVal;
}


static void switchInput(){
    userInput = getLastInput();
    switch (userInput){
        case EMPTY_BUFFER:
            break;
        case 'w':
        case 'W':
            enqueueToBuffer(UP);
            break;
        case 'd':
        case 'D':
            enqueueToBuffer(RIGHT);
            break;
        case 's':
        case 'S':
            enqueueToBuffer(DOWN);
            break;
        case 'a':
        case 'A':
            enqueueToBuffer(LEFT);
            break;
        default:  //maybe add a p for pause idk
            ;
    }
}

static void switchBuffer(){
    if(! inputBufferLoad){
        return;
    }
    Direction next = dequeueFromBuffer();
    if (currentDirection == next || (currentDirection + next) % 2 == 0){  //se ve medio feo hacer suma y modulo de dos enum pero básicamente la condicón es que no sean opuestos
        switchBuffer();
        return;
    }
    currentDirection = next; 
}

static void printTime(){
    putChar('0' + hours / 10);
    putChar('0' + hours % 10);
    putChar(':');
    putChar('0' + minutes / 10);
    putChar('0' + minutes % 10);
    putChar(':');
    putChar('0' + seconds / 10);
    putChar('0' + seconds % 10);
    
}

void game(){
    initializeGame();
    while(! endGame){
        updateTime();
        switchInput();
        currentTicks = getTicks();
        if(currentTicks - ticksAtEnlargement >= 273){
                sound = FIRST;
                playSound(5000);
                moveHead();
                moveHead();
                ticksAtEnlargement = currentTicks;
                if(currentLevel){currentLevel-- ;}
        }
        if(currentTicks - ticksAtUpdate > currentLevel){
            switchBuffer();
            ticksAtUpdate = currentTicks;
            moveSnake();
            score++;
        }
        if(currentTicks - ticksAtEnlargement > 1 && sound == SECOND){
            stopSound();
            sound = NONE;
        }
        else if(currentTicks - ticksAtEnlargement > 0 && sound == FIRST){
            stopSound();
            playSound(2500);
            sound = SECOND;
        }
    }
    
    clearScreen();
    playFrequency(5000, 30);
    print(endGameMessage);
    print(" Your score was: ");
    printDec(score);
    print(". Your game time was: ");
    printTime();
    printAction(0);
    print("Now returning to Shell");
}

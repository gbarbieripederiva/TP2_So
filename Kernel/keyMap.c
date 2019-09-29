#include <keyMap.h>

//Table to map keyboard entry to real ascii
const unsigned char asciiMap[128] = { 0, //Filler
    0x1B, //Escape
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    0x08, //Backspace
    0, //Tab
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    0x0A, //New line
    0, //Ctrl
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
    0x60, //Back tick
    0,	//Left Shift
    0x5C, //Back slash
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, //Right Shift
    '*', //Only for keypads
    0,	//Alt
    ' ', //Space bar
    0,	//Caps
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //F1 to F10
    0,	    //Num Lock
    0,	    //Scroll Lock
    0,	//Home key
    0,	//Up Arrow
    0,	//Page up
    '-',
    0,	// Left Arrow
    0,  //Filler
    0,	//Right Arrow
    '+',
    0,	//End key
    0,	//Down Arrow
    0,	//Page Down
    0,	//Insert Key
    0,	//Delete Key
    0,   0,   0, //Fillers
    0,	//F11
    0,	//F12
    0,	//Fillers
};

//Same as the other table but when shift is applied
const char shiftMap[255] = {0, 0x1B, '!', '@','#','$','%','^','&','*',
    '(',')','_','+',0x08,0x09,'Q','W','E','R','T','Y','U','I','O','P','{','}','\n'
    ,0,'A','S','D','F','G','H','J','K','L',':','"','~',0,'|','Z','X','C','V','B'
    ,'N','M','<','>','?',0,0,0,' ',0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,'-',0,0,0,'+',0,0,0};

char getAscii(unsigned int code) {
	return asciiMap[code];
}

char getShiftAscii(unsigned int code) {
    return shiftMap[code];
}

char toLowerCase(char c) {
    if(c >= 'A' && c<= 'Z') {
        c += ('a' - 'A');
    }
    return c;
}

char toUpperCase(char c) {
    if(c >= 'a' && c<= 'z') {
        c += ('A' - 'a');
    }
    return c;   
}
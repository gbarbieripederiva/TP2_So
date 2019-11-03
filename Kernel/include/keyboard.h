#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_handler(uint8_t);
uint8_t getChar();
uint8_t getLastInput();
int getCharFromKeyboardPipe();
void initKeyboardPipe();
int readFromKeyboardPipe(char *buff,int size);

#endif
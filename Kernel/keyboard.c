#include <stdint.h>
#include <keyboard.h>
#include <naiveConsole.h>
#include <keyMap.h>
#include <fd.h>

//Chose 100 as maximum size
#define BUFFER_SIZE 100

//Buffer for all the codes
static uint8_t buffer[BUFFER_SIZE] = {0};

static int startPosition = 0; //To know where in the buffer to start reading from
static int endPosition = 0;   //End of the buffer(las position is endPosition-1)
static int size = 0;

//Function to handle shift pressed.
static int LSHIFT_LOCKED = 0;
static int RSHIFT_LOCKED = 0;
static int CAPS_LOCKED = 0;
static int CTRL_LOCKED = 0;

void addToBuffer(char charToAdd);

//Ads an element to the buffer.
void keyboard_handler(uint8_t code)
{

	char charToAdd = 0;

	//If the buffer is already full
	if (size >= 100)
	{
		ncPrint("ERROR: BUFFER overflow");
	}

	if (code < END_OF_PRESSING_KEYS)
	{ //If we are pressing the keyboard(not releasing)
		//Cases where we are pressing an special key
		switch (code)
		{
		case CTRL:
			CTRL_LOCKED = 0; //temporal
			break;
		case CAPS:
			CAPS_LOCKED = !CAPS_LOCKED;
			break;
		case LSHIFT:
			LSHIFT_LOCKED = 1;
			break;
		case RSHIFT:
			RSHIFT_LOCKED = 1;
			break;
		}

		//Add the letter based on the state of our special letters
		if (LSHIFT_LOCKED || RSHIFT_LOCKED)
		{
			charToAdd = getShiftAscii(code);
			if (charToAdd != 0)
			{ //If it is a valid letter.
				if (CAPS_LOCKED)
				{
					//Case when the shift is pressed and we are in CAPS as well.
					addToBuffer(toLowerCase(charToAdd));
				}
				else
				{
					addToBuffer(charToAdd);
				}
			}
		}
		else if (CTRL_LOCKED)
		{
			//Define what we are gonna do with control
		}
		else if (CAPS_LOCKED)
		{
			charToAdd = getAscii(code);
			if (charToAdd != 0)
			{
				addToBuffer(toUpperCase(charToAdd));
			}
		}
		//Standard case, just adding the letter.
		else
		{
			charToAdd = getAscii(code);
			if (charToAdd != 0)
			{
				addToBuffer(charToAdd);
			}
		}
	}
	//This means a key was released, we only care if the key is Shift or Ctrl
	else
	{
		switch (code)
		{
		case CTRL_UP:
			CTRL_LOCKED = 0;
			break;
		case LSHIFT_UP:
			LSHIFT_LOCKED = 0;
			break;
		case RSHIFT_UP:
			RSHIFT_LOCKED = 0;
			break;
		}
	}
}

//Adds to buffer the current letter.
void addToBuffer(char charToAdd)
{
	buffer[endPosition] = charToAdd;
	fd_write(KEYBOARD,buffer + endPosition ,1);
	endPosition = (endPosition + 1) % BUFFER_SIZE; //As its cyclic iterator
	size++;
	
}

//Function to return a uint8 from the buffer and delete it. Return 0 if empty
uint8_t getChar()
{	
	if (size <= 0)
	{ //If we have an empty buffer
		return 0;
	}
	else
	{
		//uint8_t to_return = buffer[startPosition];
		//startPosition = (startPosition + 1) % BUFFER_SIZE;
		//size--;
		return get_char_fd(KEYBOARD);
		
	}
}

//Return the last input read from the keyboard and consumes it. Returns 0 if buffer empty
uint8_t getLastInput()
{
	
	/*
	if (size <= 0)
	{
		return 0;
	}
	*/
	if(endPosition == 0)
	{
		endPosition = BUFFER_SIZE-1;
		size--;
		//return buffer[endPosition];
	}
	else
	{
		endPosition--;
		size--;
		//return buffer[endPosition];
	}
	char aux;

	fd_read(KEYBOARD, &aux, 1);
	return (uint8_t)aux;
}

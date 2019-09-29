#include <naiveConsole.h>
#include <drivers/videoDrivers.h>

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

static char buffer[64] = {'0'};
static uint8_t *const video = (uint8_t *)0xB8000;
static uint8_t *currentVideo = (uint8_t *)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;

void ncPrint(const char *string)
{
	writeString((uint8_t *)string);
}

void ncPrintChar(char character)
{
	writeChar(character);
}

void ncNewline()
{
	newLine();
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
	uintToBase(value, buffer, base);
	ncPrint(buffer);
}

void ncClear()
{
	clear();
}

void ncBackspace(){
	backSpace();
}

void ncNewLine(){
	newLine();
}
//moves al lines one up
void ncMoveUp()
{
	moveScreenUp();
}


static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

/*-------------------------------------------------------------------------------------------

ALL the following functions are deprecated in text mode and should be modified to 
video mode

--------------------------------------------------------------------------------------------*/

// same as ncPrint but if it gets to the end then move up
void ncPrintAsTerminal(const char *str)
{
	for (int i = 0; str[i] != 0; i++)
	{
		if (currentVideo >= video + width * height * 2)
		{
			ncMoveUp();
		}
		ncPrintChar(str[i]);
	}
}
//------------------------------------------------------
// same as their now withColor counterparts but you can
// configure the color of backgroud and letter
// ej:0x12: 1 is the background and 2 the letter
void ncPrintCharWithColor(char character, uint8_t color)
{
	*currentVideo = character;
	currentVideo++;
	*currentVideo = color;
	currentVideo++;
}

void ncPrintWithColor(const char *string, uint8_t colors)
{
	int i;
	for (i = 0; string[i] != 0; i++)
	{
		ncPrintCharWithColor(string[i], colors);
	}
}
void ncPrintAsTerminalWithColor(const char *str, uint8_t colors)
{
	for (int i = 0; str[i] != 0; i++)
	{
		if (currentVideo >= video + width * height * 2)
		{
			ncMoveUp();
		}
		ncPrintCharWithColor(str[i], colors);
	}
}

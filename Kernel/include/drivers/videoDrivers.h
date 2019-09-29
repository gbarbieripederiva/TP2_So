#ifndef V_DRIVERS
#define V_DRIVERS

#include <stdint.h>

typedef uint32_t colour; //makes usage of calls to drawChar and drawString a lot easier, especially using 8-colour palette

#define V_BLACK 0x000000
#define V_WHITE 0xFFFFFF
#define V_RED 0x0000FF
#define V_GREEN 0x00FF00
#define V_BLUE 0xFF0000
#define V_YELLOW 0x00FFFF
#define V_MAGENTA 0xFF00FF
#define V_CYAN 0xFFFF00
#define PIXELS_PER_CHAR 8


void setCursor(uint16_t x, uint16_t y);

/* 
    Driver must be initialized if VESA mode is enabled 
*/
void initializeDriver();

/*
    Returns the real coordinate for pixel at screen coordinates (x,y)
 */
uint8_t *coordinate(uint16_t x, uint16_t y);

/*
    Draws a pixel at real coordinates parameter, with bgr encoding for colour
 */

/*
    Returns a single colour to be used with drawChar and drawString, if you want to use a colour not in the 8-colour palette
 */

colour toColour(uint8_t blue, uint8_t green, uint8_t red);

/*
    drawPixel receives unmasked colour, to avoid making unnecessary shift operations when drawing figures
 */

void drawPixel(uint8_t *coordinate, uint8_t blue, uint8_t green, uint8_t red);

/*
    Draws a Rectangle starting with the pixel at a real coordinate parameter, with length "base" on the x-axis and
    length "height" on the y-axis. Fills with bgr encoded colour
 */

void drawRectangle(uint8_t *coordinate, uint16_t base, uint16_t height, colour c);

/*
    Just a same-sided rectangle
 */

void drawSquare(uint8_t *coordinate, uint16_t side, colour c);

/*
    Draws ASCII character at specified coordinate. Character's ASCII code must not exceed 0x81. 
 */

void drawCharSized(uint8_t *coordinate, uint8_t character, uint8_t fontSize, colour foreground, colour background);

/*
    Same as above, fontSize defaulted to 1, colours defaulted to white and black
 */

void drawChar(uint8_t *coordinate, uint8_t character);
void drawCharAt(uint16_t x, uint16_t y, uint8_t character);
/*
    Draw String starting at specified coordinate. drawChar restrictions apply to the string
 */

void drawStringSized(uint8_t *coordinate, uint8_t *string, uint8_t fontSize, colour foreground, colour background);

/*
    Same as above, fontSize defaulted to 1, colours defaulted to white and black
 */

void drawString(uint8_t *coordinate, uint8_t *string);

//--------------------------CONSOLE HANDLING--------------------------------------------

void initializeConsole();

void clear();

void moveScreenUp();

void writeChar(uint8_t character);

void writeString(const uint8_t *string);

void newLine();

void backSpace();

void getScreenSize(uint16_t *width, uint16_t *height);

#endif

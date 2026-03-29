#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../graphic/Color.h"

#define T_BLACK   30
#define T_RED     31
#define T_GREEN   32
#define T_YELLOW  33
#define T_BLUE    34
#define T_MAGENTA 35
#define T_CYAN    36
#define T_WHITE   37

#ifndef TERMINAL_H
#define TERMINAL_H

// Configure terminal modes
void changeTerminalToNonCanonicalMode();
void resetTerminalMode();

// Read input character
char getChar();

// Terminal screen and cursor control
void moveCursor(unsigned int row, unsigned int column);
void clearTerminal();
void clearLine();
void saveCursor();
void restoreCursor();

//Text and background color functions
void setTextColor(unsigned int color);
void setBGColor(unsigned int color);
void setTextAndBGColor(unsigned int textColor, unsigned int backgroundColor);

void setTextRGB(Color color, int alpha);
void setBGRGB(Color color, int alpha);
void setTerminalRGB(Color color, int alpha);

void resetColor();

#endif
#include <stdlib.h>

#include "../terminal/Terminal.h"

#define COLOR_USE_ALPHA         1<<0
#define COLOR_USE_TEXT_CONTRAST 1<<1

#ifndef WINDOW_H
#define WINDOW_H

/*
Basically, the window works as follows:
    First, the terminal is initialized, switching it to non-canonical mode, etc.
    Then, two structures are created: the window and its attributes, initialized with createWindow().
    The window currently has 2 configurable flags:
        Whether to use the alpha channel, to consider or not alpha when blending colors.
        Whether to use the text contrast channel.
    Channels are stored as unsigned char pointers in the RGBACTChannels structure (Red Green Blue Alpha Contrast Text).
*/

/*
Text contrast is a feature added to this library to display text on certain pixels, useful for low-resolution games to create menus or dialogs. It can be enabled or disabled with the corresponding flag.
When enabled, a new channel is activated: the contrast channel, which determines if each pixel has contrast or not (1 or 0). If contrast is present, the display function will show the text at that pixel in white or black based on the pixel's color.
In this case, the text channel stores only the text to be displayed on screen, so if a pixel has contrast != 0, the next character from the Text Buffer is read and displayed at that pixel, saving a lot of memory.
*/

//Window attributtes structure.
typedef struct{
    unsigned int width, height;
    int color_flags;
}WindowAttributes;

//RGBACT (Red, Green, Blue, Alpha, Contrast and Text) channels structure.
typedef struct{
    unsigned char *redChannel, *greenChannel, *blueChannel, *alphaChannel;
    unsigned char *textContrast;
    char *textBuffer;
}RGBACTChannels;

//Window structure.
typedef struct{
    RGBACTChannels channels;
    WindowAttributes *attributes;

    //Used in display() function.
    //For eviting doing malloc and free so many times.
    char *displayLineBuffer;
    unsigned int displayBufferLineSize;
}Window;

//Functions for configure the terminal for the window.
void windowInitTerminal();
void windowresetTerminal();

//Creation and destruction of window.
void createWindow(WindowAttributes attributes, Window *window);
void destroyWindow(Window *window);

//Function to display the window content in the termianl.
void display(Window *window);

#endif
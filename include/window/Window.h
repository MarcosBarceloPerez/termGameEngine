#include <stdlib.h>

#include "../terminal/Terminal.h"

#define COLOR_USE_ALPHA         1<<0
#define COLOR_USE_TEXT_CONTRAST 1<<1

#ifndef WINDOW_H
#define WINDOW_H

typedef struct{
    unsigned int width, height;
    int color_flags;
}WindowAttributes;

typedef struct{
    unsigned char *redChannel, *blueChannel, *greenChannel, *alphaChannel;
    unsigned char *textContrast;
    char *textBuffer;
}RGBACTChannels;

typedef struct{
    RGBACTChannels channels;
    WindowAttributes *attributes;
}Window;

void windowInitTerminal();
void windowresetTerminal();

void createWindow(WindowAttributes attributes, Window *window);
void destroyWindow(Window *window);

void display(Window *window);

#endif
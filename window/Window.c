#include "../include/window/Window.h"

static void wFormPixelLine(RGBACTChannels currentChannels, int alpha, int textContrast, int width, char *buffer);
static void wFormPixel(RGBACTChannels currentChannels, int alpha, int textContrast, char **buffer);
static void wGenStringColor(int color, char *buffer);

void windowInitTerminal(){
    changeTerminalToNonCanonicalMode();
}

void windowresetTerminal(){
    resetTerminalMode();
}

void createWindow(WindowAttributes pAttributes, Window *window){
    WindowAttributes *attributes = NULL;

    window = malloc(sizeof(Window));
    attributes = malloc(sizeof(WindowAttributes));

    *attributes = pAttributes;

    window->attributes = attributes;

    unsigned int channelSizes = attributes->width * attributes->height;

    window->channels.redChannel = malloc(channelSizes);
    window->channels.greenChannel = malloc(channelSizes);
    window->channels.blueChannel = malloc(channelSizes);

    window->channels.alphaChannel = NULL;

    if((attributes->color_flags & COLOR_USE_ALPHA) != 0){
        window->channels.alphaChannel = malloc(channelSizes);
    }

    window->channels.textBuffer = NULL;
    window->channels.textContrast = NULL;

    if((attributes->color_flags & COLOR_USE_TEXT_CONTRAST) != 0){
        window->channels.textContrast = malloc(channelSizes);
    }
}

void destroyWindow(Window *window){
    free(window->channels.redChannel);
    free(window->channels.greenChannel);
    free(window->channels.blueChannel);

    if((window->attributes->color_flags & COLOR_USE_ALPHA) != 0){
        free(window->channels.alphaChannel);
    }

    if((window->attributes->color_flags & COLOR_USE_TEXT_CONTRAST) != 0){
        free(window->channels.textContrast);
    }

    free(window->attributes);
    free(window);
}



void display(Window *window){

}

static void wFormPixelLine(RGBACTChannels currentChannels, int alpha, int textContrast, int width, char *buffer){
    for(int i = 0; i < width; i++){
        wFormPixel(currentChannels, alpha, textContrast, &buffer);

        currentChannels.redChannel++;
        currentChannels.greenChannel++;
        currentChannels.blueChannel++;
        currentChannels.alphaChannel++;
        currentChannels.textContrast++;
    }
}

static void wFormPixel(RGBACTChannels currentChannels, int alpha, int textContrast, char **buffer){
    Color color;
    Color absoluteColor;

    color.red = *(currentChannels.redChannel);
    color.green = *(currentChannels.greenChannel);
    color.blue = *(currentChannels.blueChannel);

    if(alpha) color.alpha = *(currentChannels.alphaChannel);

    absoluteColor = getAbsoluteColor(color, alpha);

    char red[3];
    char green[3];
    char blue[3];

    wGenStringColor(absoluteColor.red, red);
    wGenStringColor(absoluteColor.green, green);
    wGenStringColor(absoluteColor.blue, blue);

    **buffer = '\033';
    (*buffer)++;

    **buffer = '[';
    (*buffer)++;

    **buffer = '3';
    (*buffer)++;

    **buffer = '8';
    (*buffer)++;

    **buffer = ';';
    (*buffer)++;

    **buffer = '2';
    (*buffer)++;

    **buffer = ';';
    (*buffer)++;

    **buffer = red[0];
    (*buffer)++;
    **buffer = red[1];
    (*buffer)++;
    **buffer = red[2];
    (*buffer)++;

    **buffer = ';';
    (*buffer)++;

    **buffer = green[0];
    (*buffer)++;
    **buffer = green[1];
    (*buffer)++;
    **buffer = green[2];
    (*buffer)++;

    **buffer = ';';
    (*buffer)++;

    **buffer = blue[0];
    (*buffer)++;
    **buffer = blue[1];
    (*buffer)++;
    **buffer = blue[2];
    (*buffer)++;

    **buffer = 'm';
    (*buffer)++;



    **buffer = '\033';
    (*buffer)++;

    **buffer = '[';
    (*buffer)++;

    **buffer = '4';
    (*buffer)++;

    **buffer = '8';
    (*buffer)++;

    **buffer = ';';
    (*buffer)++;

    **buffer = '2';
    (*buffer)++;

    **buffer = ';';
    (*buffer)++;

    **buffer = red[0];
    (*buffer)++;
    **buffer = red[1];
    (*buffer)++;
    **buffer = red[2];
    (*buffer)++;

    **buffer = ';';
    (*buffer)++;

    **buffer = green[0];
    (*buffer)++;
    **buffer = green[1];
    (*buffer)++;
    **buffer = green[2];
    (*buffer)++;

    **buffer = ';';
    (*buffer)++;

    **buffer = blue[0];
    (*buffer)++;
    **buffer = blue[1];
    (*buffer)++;
    **buffer = blue[2];
    (*buffer)++;

    **buffer = 'm';
    (*buffer)++;
}

static void wGenStringColor(int color, char *buffer){
    buffer[0] = (color / 100) + '0';
    buffer[1] = (color / 10) % 10 + '0';
    buffer[2] = (color) % 10 + '0';
}
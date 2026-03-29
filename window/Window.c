#include "../include/window/Window.h"

// Internal helpers to fill buffer with ANSI RGB sequences.
static void wFormPixelLine(RGBACTChannels *currentChannels, int alpha, int textContrast, int width, char *buffer);
static void wFormPixel(RGBACTChannels *currentChannels, int alpha, int textContrast, char **buffer);
static void wGenStringColor(int color, char *buffer);

// Initialize terminal mode for window rendering (raw input mode).
void windowInitTerminal(){
    changeTerminalToNonCanonicalMode();
}

// Reset terminal mode to its original state.
void windowResetTerminal(){
    resetTerminalMode();
    resetColor();
}

//Initialize window and channels.
Window *createWindow(WindowAttributes pAttributes){
    WindowAttributes *attributes = NULL;

    Window *window = NULL;

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

    //Used in display(), see it.
    window->displayBufferLineSize = window->attributes->width * 39; //39 is de size of a pixel encoded into ANSI in bytes.
    window->displayLineBuffer = malloc(window->displayBufferLineSize);

    return window;
}

//Destroy window and channels.
void destroyWindow(Window *window){
    free(window->channels.redChannel);
    free(window->channels.greenChannel);
    free(window->channels.blueChannel);

    free(window->displayLineBuffer);

    if((window->attributes->color_flags & COLOR_USE_ALPHA) != 0){
        free(window->channels.alphaChannel);
    }

    if((window->attributes->color_flags & COLOR_USE_TEXT_CONTRAST) != 0){
        free(window->channels.textContrast);
    }

    free(window->attributes);
    free(window);
}

//Displays window content in the terminal.
void display(Window *window){
    /*
    For a fast display, this function first builds a line of pixels encoded into ANSI in a buffer, and then displays,
    because if the function build and display pixel by pixel, it would be so slow.
    */

    /*
    The buffer (Window.displayLineBuffer) is initializated when window is created for eviting doing malloc() and free() at every frame.
    */

    char *buffer = window->displayLineBuffer;

    RGBACTChannels currentChannels = window->channels;

    int usesAlpha = window->attributes->color_flags & COLOR_USE_ALPHA;
    int usesContrast = window->attributes->color_flags & COLOR_USE_TEXT_CONTRAST;

    for(int line = 0; line < window->attributes->height; line++){
        wFormPixelLine(&currentChannels, usesAlpha, usesContrast, window->attributes->width, buffer);
        write(STDOUT_FILENO, buffer, window->displayBufferLineSize); //Print buffer into the terminal.
        //TODO: rewrite.
        printf("\n");
    }
}

//Encodes a line of pixels into a buffer using ANSI tat will be displayed
static void wFormPixelLine(RGBACTChannels *currentChannels, int alpha, int textContrast, int width, char *buffer){
    //In context: Current channels contains pointers into the RGBACT channels pointing to the pixel that display is displaying.
    for(int i = 0; i < width; i++){
        wFormPixel(currentChannels, alpha, textContrast, &buffer);

        //Steps into the next pixel.
        currentChannels->redChannel++;
        currentChannels->greenChannel++;
        currentChannels->blueChannel++;
        if(alpha) currentChannels->alphaChannel++;
        if(textContrast) currentChannels->textContrast++;
    }
}

//Encodes one pixel into a buffer using ANSI. 
static void wFormPixel(RGBACTChannels *currentChannels, int alpha, int textContrast, char **buffer){
    Color backgroundColor;
    Color absoluteBackgroundColor;
    Color textColor;

    backgroundColor.red = *(currentChannels->redChannel);
    backgroundColor.green = *(currentChannels->greenChannel);
    backgroundColor.blue = *(currentChannels->blueChannel);

    //For eviting a segmentation fault.
    if(alpha) backgroundColor.alpha = *(currentChannels->alphaChannel);

    //It gets the absolute color because terminal doesn't support opacity.
    absoluteBackgroundColor = getAbsoluteColor(backgroundColor, alpha);

    char bgRed[3];
    char bgGreen[3];
    char bgBlue[3];

    wGenStringColor(absoluteBackgroundColor.red, bgRed);
    wGenStringColor(absoluteBackgroundColor.green, bgGreen);
    wGenStringColor(absoluteBackgroundColor.blue, bgBlue);

    //Calculate text contrast.
    int usesContrast = textContrast && *(currentChannels->textContrast);
    if(usesContrast){
        textColor = getContrast(absoluteBackgroundColor, 0);
    }
    else{
        textColor = absoluteBackgroundColor;
    }

    char txtRed[3];
    char txtGreen[3];
    char txtBlue[3];

    wGenStringColor(textColor.red, txtRed);
    wGenStringColor(textColor.green, txtGreen);
    wGenStringColor(textColor.blue, txtBlue);

    //Encode the colors into ANSI.
    //Encoding: "\033[38;2;R;G;Bm\033[48;2;R;G;Bm" being R red, G green, B blue, and thern the character ' ' that is the pixel.
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

    **buffer = txtRed[0];
    (*buffer)++;
    **buffer = txtRed[1];
    (*buffer)++;
    **buffer = txtRed[2];
    (*buffer)++;

    **buffer = ';';
    (*buffer)++;

    **buffer = txtGreen[0];
    (*buffer)++;
    **buffer = txtGreen[1];
    (*buffer)++;
    **buffer = txtGreen[2];
    (*buffer)++;

    **buffer = ';';
    (*buffer)++;

    **buffer = txtBlue[0];
    (*buffer)++;
    **buffer = txtBlue[1];
    (*buffer)++;
    **buffer = txtBlue[2];
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

    **buffer = bgRed[0];
    (*buffer)++;
    **buffer = bgRed[1];
    (*buffer)++;
    **buffer = bgRed[2];
    (*buffer)++;

    **buffer = ';';
    (*buffer)++;

    **buffer = bgGreen[0];
    (*buffer)++;
    **buffer = bgGreen[1];
    (*buffer)++;
    **buffer = bgGreen[2];
    (*buffer)++;

    **buffer = ';';
    (*buffer)++;

    **buffer = bgBlue[0];
    (*buffer)++;
    **buffer = bgBlue[1];
    (*buffer)++;
    **buffer = bgBlue[2];
    (*buffer)++;

    **buffer = 'm';
    (*buffer)++;

    if(!usesContrast) **buffer = ' ';
    else{
        **buffer = *currentChannels->textBuffer;
        currentChannels->textBuffer++;
    }
    (*buffer)++;
}

//Creates a char[3], transforming a number into a string.
static void wGenStringColor(int color, char *buffer){
    buffer[0] = (color / 100) + '0';
    buffer[1] = (color / 10) % 10 + '0';
    buffer[2] = (color) % 10 + '0';
}
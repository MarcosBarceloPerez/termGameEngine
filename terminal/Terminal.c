#include "../include/terminal/Terminal.h"

//Configure terminal

struct termios oldTerm;

void changeTerminalToNonCanonicalMode(){
    struct termios newTerm;

    tcgetattr(STDIN_FILENO, &oldTerm);

    newTerm = oldTerm;

    newTerm.c_lflag &= ~ICANON;
    newTerm.c_lflag &= ~ECHO;

    newTerm.c_cc[VMIN] = 0;
    newTerm.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);
}

void resetTerminalMode(){
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
}

//Read terminal

char getChar(){
    char character = 0;
    read(STDIN_FILENO, &character, 1);
    return character;
}

//Manipulate terminal

void moveCursor(unsigned int row, unsigned int column){
    printf("\033[%u,%uH", row, column);
}

void clearTerminal(){
    printf("\033[2J");
}

void clearLine(){
    printf("\033[2K");
}

void saveCursor(){
    printf("\033[s");
}

void restoreCursor(){
    printf("\033[u");
}



void setTextColor(unsigned int color){
    printf("\033[%um", color);
}

void setBGColor(unsigned int color){
    printf("\033[%um", color + 10);
}

void setTextAndBGColor(unsigned int textColor, unsigned int backgroundColor){
    printf("\033[%i;%im", textColor, backgroundColor + 10);
}

void resetColor(){
    printf("\033[0m");
}


void setTextRGB(Color color, int alpha){
    Color absoluteColor = getAbsoluteColor(color, alpha);

    printf("\033[38;2;%i;%i;%im", absoluteColor.red, absoluteColor.green, absoluteColor.blue);
}

void setBGRGB(Color color, int alpha){
    Color absoluteColor = getAbsoluteColor(color, alpha);

    printf("\033[48;2;%i;%i;%im", absoluteColor.red, absoluteColor.green, absoluteColor.blue);
}

void setTerminalRGB(Color color, int alpha){
    Color absoluteColor = getAbsoluteColor(color, alpha);

    printf("\033[38;2;%i;%i;%im\033[48;2;%i;%i;%im", absoluteColor.red, absoluteColor.green, absoluteColor.blue, 
                                                     absoluteColor.red, absoluteColor.green, absoluteColor.blue);
}
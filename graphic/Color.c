#include "../include/graphic/Color.h"

Color getAbsoluteColor(Color color, int alpha){
    Color absoluteColor;
    absoluteColor.alpha = 255;

    if(!alpha){
        absoluteColor.red = color.red;
        absoluteColor.green = color.green;
        absoluteColor.blue = color.blue;
    }
    else{
        float alphaFactor = ((float)color.alpha/255.0f);
        absoluteColor.red = color.red * alphaFactor;
        absoluteColor.green = color.green * alphaFactor;
        absoluteColor.blue = color.blue * alphaFactor;
    }

    return absoluteColor;
}
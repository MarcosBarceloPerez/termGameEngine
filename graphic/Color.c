#include "../include/graphic/Color.h"

Color getAbsoluteColor(Color color, int alpha){
    // Convert color to full opacity RGB version.
    // If alpha == 0, the original color is returned unchanged.
    // If alpha != 0, the color channels are scaled by alpha parameter.
    Color absoluteColor;
    absoluteColor.alpha = 255;

    if(!alpha){
        absoluteColor.red = color.red;
        absoluteColor.green = color.green;
        absoluteColor.blue = color.blue;
    }
    else{
        float alphaFactor = ((float)color.alpha / 255.0f);
        absoluteColor.red = color.red * alphaFactor;
        absoluteColor.green = color.green * alphaFactor;
        absoluteColor.blue = color.blue * alphaFactor;
    }

    return absoluteColor;
}
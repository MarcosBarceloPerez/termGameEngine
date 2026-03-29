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

//Calculates the contrast color (black or white) with a weighted sum, of another color.
Color getContrast(Color color, int alpha){
    Color contrast;
    Color absoluteColor = getAbsoluteColor(color, alpha);

    int distanceSQ = 0.299 * color.red + 0.587 * color.green + 0.114 * color.blue;
    
    if(distanceSQ >= 128){
        contrast.red = 0;
        contrast.green = 0;
        contrast.blue = 0;
    }
    else{
        contrast.red = 255;
        contrast.green = 255;
        contrast.blue = 255;
    }

    return contrast;
}
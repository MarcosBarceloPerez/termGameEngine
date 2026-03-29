#ifndef COLOR_H
#define COLOR_H

typedef struct{
    unsigned char red, green, blue, alpha;
}Color;

// Convert an RGBA color to non-transparent RGB.
Color getAbsoluteColor(Color color, int alpha);

//Calculate a color (black or white) that contrast with an other color.
Color getContrast(Color color, int alpha);

#endif
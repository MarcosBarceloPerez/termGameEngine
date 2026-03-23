#ifndef COLOR_H
#define COLOR_H

typedef struct{

    unsigned char red, green, blue, alpha;

}Color;

Color getAbsoluteColor(Color color, int alpha);

#endif
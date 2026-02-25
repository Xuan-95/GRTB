#ifndef COLOR_H
#define COLOR_H

#include "vector3d.h"
#include <stdio.h>

typedef struct {
    int r;
    int g;
    int b;
} Color;

void writeColor(FILE *file, Color color);
Color toColor(Vector3D vec);

#endif // COLOR_H

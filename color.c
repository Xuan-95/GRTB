#include "color.h"
#include "vector3d.h"
#include <stdio.h>

void writeColor(FILE *file, Color color) {
    fprintf(file, "%d %d %d\n", color.r, color.g, color.b);
}

// Convert a 3D vector with values between 0 and 1 in a Color with value between
// 0 and 255
Color toColor(Vector3D vec) {
    Color color;
    color.r = (int)(255.999 * vec.x);
    color.g = (int)(255.999 * vec.y);
    color.b = (int)(255.999 * vec.z);
    return color;
}

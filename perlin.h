#ifndef PERLIN_H
#define PERLIN_H

#include "common.h"

#define PERLIN_POINT_COUNT 255

typedef struct {
    double randfloat[PERLIN_POINT_COUNT];
    int    perm_x[PERLIN_POINT_COUNT];
    int    perm_y[PERLIN_POINT_COUNT];
    int    perm_z[PERLIN_POINT_COUNT];
} Perlin;

Perlin *createPerlin(void);
double  noise(Perlin *perlin, Point3D p);
void    perlinGeneratePerm(int *p);
void    permute(int *p, int n);

#endif

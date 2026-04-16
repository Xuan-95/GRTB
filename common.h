#ifndef COMMON_H
#define COMMON_H

#include "interval.h"
#include "memory.h"
#include "ray.h"
#include "vector3d.h"
#include <math.h>
#include <stdlib.h>

#define UNUSED(x) (void)(x)

#define PI        3.1415926535897932385

static inline double degrees_to_radians(double degrees) { return degrees * PI / 180.0; }
static inline double randomDouble(double min, double max) { return min + (max - min) * (rand() / (RAND_MAX + 1.0)); }
static inline int    randomInt(int min, int max) { return (int)randomDouble(min, max + 1); }

unsigned char        floatToByte(float value);
int                  clamp(int x, int low, int high);

#endif

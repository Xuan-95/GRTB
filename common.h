#ifndef COMMON_H
#define COMMON_H

// Common Headers
#include <math.h>
#include <stdlib.h>

#include "memory.h"
#include "ray.h"
#include "vector3d.h"
#include "interval.h"

// Constants
#define PI 3.1415926535897932385

// Utility Functions
static inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

static inline double randomDouble(double min, double max) {
    return min + (max - min) * (rand() / (RAND_MAX + 1.0));
}

#endif

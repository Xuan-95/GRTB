#ifndef COMMON_H
#define COMMON_H

// Common Headers
#include <math.h>

#include "ray.h"
#include "vector3d.h"

// Constants
#define PI 3.1415926535897932385

// Utility Functions
static inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

#endif

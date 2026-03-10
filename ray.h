#ifndef RAY_H
#define RAY_H

#include "vector3d.h"

typedef struct {
    Point3D  origin;
    Vector3D direction;
    double   time;
} Ray;

Ray     createRay(Point3D origin, Vector3D direction, double time);
Point3D rayAt(Ray ray, double t);

#endif // !RAY_H
#define RAY_H

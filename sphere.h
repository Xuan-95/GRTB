#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vector3d.h"

typedef struct {
    Hittable base;
    Point3D center;
    double radius;
} Sphere;

int hitSphere(Hittable *self, Ray *r, double ray_tmin, double ray_tmax,
              HitRecord *rec);

#endif

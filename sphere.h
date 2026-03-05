#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "hittable.h"

typedef struct {
    Hittable base;
    Point3D center;
    double radius;
    Material *mat;
} Sphere;

Hittable *createSphere(Point3D center, double radius, Material* mat);
int hitSphere(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);

#endif

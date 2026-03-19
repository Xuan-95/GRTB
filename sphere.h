#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "hittable.h"

typedef struct {
    Hittable  base;
    Ray       center;
    double    radius;
    Material *mat;
    int       is_moving;
} Sphere;

Hittable *createSphere(Point3D center, double radius, Material *mat);
Hittable *createMovingSphere(Point3D center_1, Point3D center_2, double radius, Material *mat);
int       hitSphere(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
void      getSphereUV(Point3D p, double *u, double *v);

#endif

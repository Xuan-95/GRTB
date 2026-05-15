#ifndef SPHERE_H
#define SPHERE_H

#include "../core/common.h"
#include "hittable.h"

Hittable *createSphere(Point3D center, double radius, Material *mat);
Hittable *createMovingSphere(Point3D center_1, Point3D center_2, double radius, Material *mat);
int       hitSphere(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
void      getSphereUV(Point3D p, double *u, double *v);
double    spherePdfValue(Hittable *self, Point3D origin, Vector3D direction);
Vector3D  spherePdfRandom(Hittable *self, Vector3D origin);
Vector3D  randomToSphere(double radius, double distance_squared);

#endif

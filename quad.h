#ifndef QUAD_H
#define QUAD_H

#include "common.h"
#include "hittable.h"
#include "hittable_list.h"

typedef struct {
    Hittable  base;
    Point3D   Q;
    Vector3D  u;
    Vector3D  v;
    Vector3D  normal;
    Vector3D  w;
    double    D;
    Material *mat;
} Quad;

Hittable     *createQuad(Point3D Q, Vector3D u, Vector3D v, Material *mat);
int           hitQuad(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
HittableList *createBox(Point3D a, Point3D b, Material *mat);

#endif

#ifndef HITTABLE_H
#define HITTABLE_H

#include "common.h"

typedef struct Hittable Hittable;
typedef struct Material Material;

typedef struct {
    Point3D   p;
    Vector3D  normal;
    Material *mat;
    double    t;
    int       front_face;
} HitRecord;

struct Hittable {
    int (*hit)(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
};

void      setFaceNormal(HitRecord *rec, Ray *r, Vector3D outward_normal);
HitRecord createHitRecord(Point3D p, Vector3D normal, double t, int front_face);

#endif // !HITTABLE_H

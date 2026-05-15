#ifndef QUAD_H
#define QUAD_H

#include "../core/common.h"
#include "hittable.h"
#include "hittable_list.h"

Hittable     *createQuad(Point3D Q, Vector3D u, Vector3D v, Material *mat);
int           hitQuad(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
void          addBox(HittableList *list, Point3D a, Point3D b, Material *mat);
HittableList *createBox(Point3D a, Point3D b, Material *mat);
double        quadPdfValue(Hittable *self, Point3D origin, Vector3D direction);
Vector3D      quadPdfRandom(Hittable *self, Vector3D origin);

#endif

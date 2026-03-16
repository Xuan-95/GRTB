#ifndef BVH_H
#define BVH_H

#include "hittable.h"
#include "hittable_list.h"

typedef struct {
    Hittable  base;
    Hittable *left;
    Hittable *right;
} Bvh;

Hittable *createBvh(Hittable **objects, size_t start, size_t end);
Hittable *createBvhFromList(HittableList *list);
int       hitBvh(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
int       boxCompare(Hittable *a, Hittable *b, int axis_index);
int       boxXCompare(const void *a, const void *b);
int       boxYCompare(const void *a, const void *b);
int       boxZCompare(const void *a, const void *b);

#endif // !BVH_H

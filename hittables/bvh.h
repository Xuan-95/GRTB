#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

typedef struct {
    Aabb bbox;
    int  second_child;
    int  prim_idx; // if >=0 is leaf, -1 is internal node
} LinearBvhNode;

typedef struct {
    Hittable       base;
    LinearBvhNode *nodes;
    Hittable     **primitives;
    int            node_count;
    int            prim_count;
} LinearBvh;

Hittable *createLinearBvh(Hittable **objects, size_t size);
Hittable *createLinearBvhFromList(HittableList *list);
int       hitLinearBvh(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
int       boxCompare(Hittable *a, Hittable *b, int axis_index);
int       boxXCompare(const void *a, const void *b);
int       boxYCompare(const void *a, const void *b);
int       boxZCompare(const void *a, const void *b);

#endif // !BVH_H

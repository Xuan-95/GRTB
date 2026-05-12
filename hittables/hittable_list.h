#ifndef HITTABLE_LIST
#define HITTABLE_LIST

#include "hittable.h"
#include "sphere.h"

typedef struct {
    Hittable   base;
    Hittable **objects;
    int        count;
    int        capacity;
} HittableList;

void     initHittableList(HittableList *hittable_list);
void     addObject(HittableList *hittable_list, Hittable *hittable);
int      hitHittableList(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
double   hittableListPdfValue(Hittable *self, Point3D origin, Vector3D direction);
Vector3D hittableListRandom(Hittable *self, Point3D origin);

#endif // !HITTABLE_LIST

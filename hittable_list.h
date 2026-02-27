#ifndef HITTABLE_LIST
#define HITTABLE_LIST

#include "hittable.h"
#include "sphere.h"
typedef struct {
    Hittable base;
    Hittable **objects;
    int count;
    int capacity;
} HittableList;

void initHittableList(HittableList *hittable_list);
void addObject(HittableList *hittable_list, Hittable *hittable);
int hitHittableList(Hittable *self, Ray *r, double ray_tmin, double ray_tmax,
                    HitRecord *rec);

#endif // !HITTABLE_LIST

#include "hittable_list.h"
#include "hittable.h"
#include "memory.h"

void initHittableList(HittableList *hittable_list) {
    hittable_list->objects = NULL;
    hittable_list->capacity = 0;
    hittable_list->count = 0;
}

void addObject(HittableList *hittable_list, Hittable *hittable) {
    if (hittable_list->capacity < hittable_list->count - 1) {
        int oldCapacity = hittable_list->capacity;
        hittable_list->capacity = GROW_CAPACITY(oldCapacity);
        hittable_list->objects =
            GROW_ARRAY(Hittable, hittable_list->objects, oldCapacity,
                       hittable_list->capacity);
    }
    hittable_list->objects[hittable_list->count] = hittable;
    hittable_list->count++;
}

int hitHittableList(Hittable *self, Ray *r, double ray_tmin, double ray_tmax,
                    HitRecord *rec) {
    HittableList *hittable_list = (HittableList *)self;
    int hit_anything = 0;
    double closest_so_far = 0;

    HitRecord temp_rec;

    for (int i = 0; i < hittable_list->count; i++) {
        Hittable *object = hittable_list->objects[i];
        if (object->hit(object, r, closest_so_far, ray_tmax, &temp_rec)) {
            hit_anything = 1;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }
    return hit_anything;
}

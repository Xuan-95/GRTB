#include "hittable_list.h"
#include "aabb.h"
#include "common.h"
#include "interval.h"
#include "pdf.h"

void initHittableList(HittableList *hittable_list) {
    hittable_list->base.hit  = hitHittableList;
    hittable_list->base.bbox = createAabb(createInterval(INFINITY, -INFINITY), createInterval(INFINITY, -INFINITY),
                                          createInterval(INFINITY, -INFINITY));

    hittable_list->base.random   = hittableListRandom;
    hittable_list->base.pdfValue = hittableListPdfValue;

    hittable_list->objects  = NULL;
    hittable_list->capacity = 0;
    hittable_list->count    = 0;
}

void addObject(HittableList *hittable_list, Hittable *hittable) {
    if (hittable_list->count >= hittable_list->capacity) {
        int oldCapacity         = hittable_list->capacity;
        hittable_list->capacity = GROW_CAPACITY(oldCapacity);
        hittable_list->objects  = GROW_ARRAY(Hittable *, hittable_list->objects, oldCapacity, hittable_list->capacity);
    }
    hittable_list->objects[hittable_list->count] = hittable;
    hittable_list->base.bbox                     = unionAabb(hittable_list->base.bbox, hittable->bbox);
    hittable_list->count++;
}

int hitHittableList(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec) {
    HittableList *hittable_list  = (HittableList *)self;
    int           hit_anything   = 0;
    double        closest_so_far = ray_t.max;

    HitRecord     temp_rec;

    for (int i = 0; i < hittable_list->count; i++) {
        Hittable *object = hittable_list->objects[i];
        if (object->hit(object, r, createInterval(ray_t.min, closest_so_far), &temp_rec)) {
            hit_anything   = 1;
            closest_so_far = temp_rec.t;
            *rec           = temp_rec;
        }
    }
    return hit_anything;
}

double hittableListPdfValue(Hittable *self, Point3D origin, Vector3D direction) {
    HittableList *hittable_list = (HittableList *)self;
    if (hittable_list->count == 0)
        return 0.0;

    double sum = 0.0;
    for (int i = 0; i < hittable_list->count; i++) {
        sum += hittable_list->objects[i]->pdfValue(hittable_list->objects[i], origin, direction);
    }
    return sum / hittable_list->count;
}

Vector3D hittableListRandom(Hittable *self, Point3D origin) {
    HittableList *list = (HittableList *)self;
    if (list->count == 0)
        return createVector3D(1, 0, 0);

    int       index  = randomInt(0, list->count - 1);
    Hittable *target = list->objects[index];

    return target->random(target, origin);
}

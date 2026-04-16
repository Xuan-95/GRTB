#include "bvh.h"
#include "aabb.h"
#include "memory.h"

typedef int (*Comparator)(const void *, const void *);

int boxCompare(Hittable *a, Hittable *b, int axis_index) {
    Interval a_axis_interval, b_axis_interval;

    switch (axis_index) {
    case 0: {
        a_axis_interval = a->bbox.x;
        b_axis_interval = b->bbox.x;
        break;
    }
    case 1: {
        a_axis_interval = a->bbox.y;
        b_axis_interval = b->bbox.y;
        break;
    }
    case 2: {
        a_axis_interval = a->bbox.z;
        b_axis_interval = b->bbox.z;
        break;
    }
    }
    return a_axis_interval.min < b_axis_interval.min;
}
int boxXCompare(const void *a, const void *b) {
    Hittable *ha = *(Hittable **)a;
    Hittable *hb = *(Hittable **)b;
    return boxCompare(ha, hb, 0);
}
int boxYCompare(const void *a, const void *b) {
    Hittable *ha = *(Hittable **)a;
    Hittable *hb = *(Hittable **)b;
    return boxCompare(ha, hb, 1);
}
int boxZCompare(const void *a, const void *b) {
    Hittable *ha = *(Hittable **)a;
    Hittable *hb = *(Hittable **)b;
    return boxCompare(ha, hb, 2);
}

Hittable *createBvh(Hittable **objects, size_t start, size_t end) {
    Bvh *bvh = ALLOCATE(Bvh, 1);

    bvh->base.bbox = createEmptyAabb();
    for (size_t object_index = start; object_index < end; object_index++) {
        bvh->base.bbox = unionAabb(bvh->base.bbox, objects[object_index]->bbox);
    }

    int        axis       = longestAxis(bvh->base.bbox);
    Comparator comparator = (axis == 0) ? boxXCompare : (axis == 1) ? boxYCompare : boxZCompare;
    size_t     span       = end - start;

    if (span == 1) {
        bvh->left  = objects[start];
        bvh->right = objects[start];
    } else if (span == 2) {
        bvh->left  = objects[start];
        bvh->right = objects[start + 1];
    } else {
        qsort(&objects[start], span, sizeof(Hittable *), comparator);
        size_t mid = start + (int)(span / 2);
        bvh->left  = createBvh(objects, start, mid);
        bvh->right = createBvh(objects, mid, end);
    }

    bvh->base.hit = hitBvh;

    // TODO: NotImplemented -> Default Implementation
    bvh->base.random   = hittableDefaultRandom;
    bvh->base.pdfValue = hittableDefaultPdfValue;

    return (Hittable *)bvh;
}

int hitBvh(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec) {
    Bvh *bvh      = (Bvh *)self;
    int  aabb_hit = hitAabb(&bvh->base.bbox, *r, ray_t);
    if (!aabb_hit)
        return 0;

    HitRecord right_rec;
    int       hit_left = bvh->left->hit(bvh->left, r, ray_t, rec);
    int       hit_right =
        bvh->right->hit(bvh->right, r, createInterval(ray_t.min, hit_left ? rec->t : ray_t.max), &right_rec);
    if (hit_right)
        *rec = right_rec;
    return hit_left || hit_right;
}

Hittable *createBvhFromList(HittableList *list) { return createBvh(list->objects, 0, list->count); }

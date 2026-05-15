#include "bvh.h"
#include "../core/memory.h"
#include "hittable.h"
#include <string.h>

typedef int (*Comparator)(const void *, const void *);

int boxCompare(Hittable *a, Hittable *b, int axis_index) {
    double a_min, b_min;
    switch (axis_index) {
    case 0: {
        a_min = a->bbox.x.min;
        b_min = b->bbox.x.min;
        break;
    }
    case 1: {
        a_min = a->bbox.y.min;
        b_min = b->bbox.y.min;
        break;
    }
    default: {
        a_min = a->bbox.z.min;
        b_min = b->bbox.z.min;
        break;
    }
    }
    if (a_min < b_min)
        return -1;
    if (a_min > b_min)
        return 1;
    return 0;
}

int boxXCompare(const void *a, const void *b) {
    Hittable *ha = (Hittable *)a;
    Hittable *hb = (Hittable *)b;
    return boxCompare(ha, hb, 0);
}
int boxYCompare(const void *a, const void *b) {
    Hittable *ha = (Hittable *)a;
    Hittable *hb = (Hittable *)b;
    return boxCompare(ha, hb, 1);
}
int boxZCompare(const void *a, const void *b) {
    Hittable *ha = (Hittable *)a;
    Hittable *hb = (Hittable *)b;
    return boxCompare(ha, hb, 2);
}

static inline void buildLinear(Hittable *lbvh, Hittable *objects, size_t start, size_t end) {
    int            me   = lbvh->data.bvh.node_count++;
    LinearBvhNode *node = &lbvh->data.bvh.nodes[me];

    node->bbox = AABB_EMPTY;
    for (size_t object_index = start; object_index < end; object_index++) {
        node->bbox = unionAabb(node->bbox, objects[object_index].bbox);
    }

    size_t span = end - start;

    // Base case
    if (span == 1) {
        node->prim_idx                                         = lbvh->data.bvh.prim_count;
        node->second_child                                     = -1;
        lbvh->data.bvh.primitives[lbvh->data.bvh.prim_count++] = objects[start];
        return;
    }

    // Split along the widest dimension to keep the tree balanced
    int        axis       = longestAxis(node->bbox);
    Comparator comparator = (axis == 0) ? boxXCompare : (axis == 1) ? boxYCompare : boxZCompare;

    // Sort objects and recursively split the list in half
    qsort(&objects[start], span, sizeof(Hittable), comparator);
    size_t mid     = start + (int)(span / 2);
    node->prim_idx = -1;

    // Left child (will be always me + 1)
    buildLinear(lbvh, objects, start, mid);

    // Right child
    node->second_child = lbvh->data.bvh.node_count;
    buildLinear(lbvh, objects, mid, end);
}

Hittable *createLinearBvh(Hittable *objects, size_t n) {
    Hittable *lbvh = ALLOCATE(Hittable, 1);

    lbvh->type                = HITTABLE_BVH;
    lbvh->data.bvh.nodes      = ALLOCATE(LinearBvhNode, 2 * n);
    lbvh->data.bvh.primitives = ALLOCATE(Hittable, n);
    lbvh->data.bvh.node_count = 0;
    lbvh->data.bvh.prim_count = 0;

    Hittable *tmp_objs = ALLOCATE(Hittable, n);
    memcpy(tmp_objs, objects, n * sizeof(Hittable));

    buildLinear(lbvh, tmp_objs, 0, n);

    if (lbvh->data.bvh.node_count > 0) {
        lbvh->bbox = lbvh->data.bvh.nodes[0].bbox;
    }

    free(tmp_objs);
    return (Hittable *)lbvh;
}

int hitLinearBvh(Hittable *lbvh, Ray *r, Interval ray_t, HitRecord *rec) {
    LinearBvhNode *nodes = lbvh->data.bvh.nodes;

    int            stack[64];
    int            top = 0, cur = 0, hit_anything = 0;

    for (;;) {
        LinearBvhNode *nd = &nodes[cur];

        if (hitAabb(&nd->bbox, *r, ray_t)) {
            if (nd->prim_idx >= 0) { // Leaf
                Hittable *p = &lbvh->data.bvh.primitives[nd->prim_idx];
                HitRecord temp_rec;
                if (hittableHit(p, r, ray_t, &temp_rec)) {
                    hit_anything = 1;
                    ray_t.max    = temp_rec.t; // Reduce interval for next nodes
                    *rec         = temp_rec;
                }
                if (top == 0)
                    break;
                cur = stack[--top];
            } else { // Internal node
                stack[top++] = nd->second_child;
                cur          = cur + 1;
            }
        } else {
            if (top == 0)
                break;
            cur = stack[--top];
        }
    }
    return hit_anything;
}

Hittable *createLinearBvhFromList(HittableList *list) { return createLinearBvh(list->objects, list->count); }

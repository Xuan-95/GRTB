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

static inline void buildLinear(Scene *scene, Hittable *objects, size_t start, size_t end) {
    int            me   = scene->node_count++;
    LinearBvhNode *node = &scene->nodes[me];

    node->bbox = AABB_EMPTY;
    for (size_t object_index = start; object_index < end; object_index++) {
        node->bbox = unionAabb(node->bbox, objects[object_index].bbox);
    }

    size_t span = end - start;

    // Base case
    if (span == 1) {
        node->prim_idx                         = scene->prim_count;
        node->second_child                     = -1;
        scene->primitives[scene->prim_count++] = objects[start];
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
    buildLinear(scene, objects, start, mid);

    // Right child
    node->second_child = scene->node_count;
    buildLinear(scene, objects, mid, end);
}

Scene *createScene(Hittable *objects, size_t n) {
    Scene *scene = ALLOCATE(Scene, 1);

    scene->nodes      = ALLOCATE(LinearBvhNode, 2 * n);
    scene->primitives = ALLOCATE(Hittable, n);
    scene->node_count = 0;
    scene->prim_count = 0;

    Hittable *tmp_objs = ALLOCATE(Hittable, n);
    memcpy(tmp_objs, objects, n * sizeof(Hittable));

    buildLinear(scene, tmp_objs, 0, n);

    free(tmp_objs);
    return scene;
}

int hitScene(Scene *scene, Ray *r, Interval ray_t, HitRecord *rec) {
    LinearBvhNode *nodes = scene->nodes;

    int            stack[64];
    int            top = 0, cur = 0, hit_anything = 0;

    for (;;) {
        LinearBvhNode *nd = &nodes[cur];

        if (hitAabb(&nd->bbox, *r, ray_t)) {
            if (nd->prim_idx >= 0) { // Leaf
                Hittable *p = &scene->primitives[nd->prim_idx];
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

Scene *createSceneFromList(HittableList *list) { return createScene(list->objects, list->count); }

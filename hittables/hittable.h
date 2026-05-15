#ifndef HITTABLE_H
#define HITTABLE_H

#include "../core/common.h"
#include "../math/vector3d.h"
#include "../textures/texture.h"
#include "aabb.h"

typedef struct Hittable Hittable;
typedef struct Material Material;

typedef struct {
    Point3D   p;
    Vector3D  normal;
    Material *mat;
    double    t;
    double    u;
    double    v;
    int       front_face;
} HitRecord;

typedef enum {
    HITTABLE_SPHERE,
    HITTABLE_QUAD,
    HITTABLE_TRIANGLE,
    HITTABLE_TRANSLATE,
    HITTABLE_ROTATE_Y,
    HITTABLE_MEDIUM,
    HITTABLE_BVH,
    HITTABLE_HITLIST,
} HittableType;

typedef struct {
    Aabb bbox;
    int  second_child;
    int  prim_idx; // if >=0 is leaf, -1 is internal node
} LinearBvhNode;

typedef struct {
    Point3D   Q;
    Vector3D  u;
    Vector3D  v;
    Vector3D  normal;
    Vector3D  w;
    double    D;
    double    area;
    Material *mat;
} QuadData;

typedef struct {
    Ray       center;
    double    radius;
    Material *mat;
    int       is_moving;
} SphereData;

struct Hittable {
    HittableType type;
    Aabb         bbox;
    int          mat_id;
    union {
        SphereData sphere;
        QuadData   quad;
        struct {
            Hittable *object;
            Vector3D  offset;
        } translate;
        struct {
            Hittable *object;
            double    sin_theta;
            double    cos_theta;
        } rotate_y;
        struct {
            Hittable *boundary;
            double    neg_inv_density;
            Material *phase_function;
        } constant_medium;
        struct {
            LinearBvhNode *nodes;
            Hittable      *primitives;
            int            node_count;
            int            prim_count;
        } bvh;
    } data;
};

double    hittablePdfValue(Hittable *self, Point3D origin, Vector3D direction);
Vector3D  hittableRandom(Hittable *self, Vector3D origin);

void      setFaceNormal(HitRecord *rec, Ray *r, Vector3D outward_normal);
HitRecord createHitRecord(Point3D p, Vector3D normal, double t, int front_face);
Hittable *createTranslate(Hittable *object, Vector3D offset);
int       hitTranslate(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
Hittable *createRotateY(Hittable *object, double angle);
int       hitRotateY(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
Hittable *createConstantMedium(Hittable *boundary, double density, Texture *texture);
Hittable *createConstantMediumFromColor(Hittable *boundary, double density, Color color);
int       hitConstantMedium(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);

// Forward declarations for hit functions
int               hitSphere(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
int               hitQuad(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
int               hitLinearBvh(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
int               hitHittableList(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);

static inline int hittableHit(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec) {
    switch (self->type) {
    case HITTABLE_SPHERE:
        return hitSphere(self, r, ray_t, rec);
    case HITTABLE_QUAD:
        return hitQuad(self, r, ray_t, rec);
    case HITTABLE_BVH:
        return hitLinearBvh(self, r, ray_t, rec);
    case HITTABLE_TRANSLATE:
        return hitTranslate(self, r, ray_t, rec);
    case HITTABLE_ROTATE_Y:
        return hitRotateY(self, r, ray_t, rec);
    case HITTABLE_MEDIUM:
        return hitConstantMedium(self, r, ray_t, rec);
    case HITTABLE_HITLIST:
        return hitHittableList(self, r, ray_t, rec);
    default:
        return 0;
    }
}

#endif // !HITTABLE_H

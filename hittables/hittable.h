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
    HITTABLE_MEDIUM,
    HITTABLE_HITLIST,
} HittableType;

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
            Hittable *boundary;
            double    neg_inv_density;
            Material *phase_function;
        } constant_medium;
    } data;
};

double    hittablePdfValue(Hittable *self, Point3D origin, Vector3D direction);
Vector3D  hittableRandom(Hittable *self, Vector3D origin);

void      setFaceNormal(HitRecord *rec, Ray *r, Vector3D outward_normal);
HitRecord createHitRecord(Point3D p, Vector3D normal, double t, int front_face);
Hittable *createConstantMedium(Hittable *boundary, double density, Texture *texture);
Hittable *createConstantMediumFromColor(Hittable *boundary, double density, Color color);
int       hitConstantMedium(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);

// Forward declarations for hit functions
int               hitSphere(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
int               hitQuad(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
int               hitHittableList(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);

static inline int hittableHit(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec) {
    switch (self->type) {
    case HITTABLE_SPHERE:
        return hitSphere(self, r, ray_t, rec);
    case HITTABLE_QUAD:
        return hitQuad(self, r, ray_t, rec);
    case HITTABLE_MEDIUM:
        return hitConstantMedium(self, r, ray_t, rec);
    case HITTABLE_HITLIST:
        return hitHittableList(self, r, ray_t, rec);
    default:
        return 0;
    }
}

#endif // !HITTABLE_H

#ifndef HITTABLE_H
#define HITTABLE_H

#include "aabb.h"
#include "common.h"
#include "texture.h"
#include "vector3d.h"

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

struct Hittable {
    int (*hit)(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
    Aabb bbox;
    double (*pdfValue)(Hittable *self, Point3D origin, Vector3D direction);
    Vector3D (*random)(Hittable *self, Vector3D origin);
};

typedef struct {
    Hittable  base;
    Hittable *object;
    Vector3D  offset;
} Translate;

typedef struct {
    Hittable  base;
    Hittable *object;
    double    cos_theta;
    double    sin_theta;
} RotateY;

typedef struct {
    Hittable  base;
    Hittable *boundary;
    double    neg_inv_density;
    Material *phase_function;
} ConstantMedium;

void                   setFaceNormal(HitRecord *rec, Ray *r, Vector3D outward_normal);
HitRecord              createHitRecord(Point3D p, Vector3D normal, double t, int front_face);
Hittable              *createTranslate(Hittable *object, Vector3D offset);
int                    hitTranslate(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
Hittable              *createRotateY(Hittable *object, double angle);
int                    hitRotateY(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);
Hittable              *createConstantMedium(Hittable *boundary, double density, Texture *texture);
Hittable              *createConstantMediumFromColor(Hittable *boundary, double density, Color color);
int                    hitConstantMedium(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);

static inline double   hittableDefaultPdfValue(Hittable *self, Point3D origin, Vector3D direction) { UNUSED(self); UNUSED(origin); UNUSED(direction); return 0.0; }
static inline Vector3D hittableDefaultRandom(Hittable *self, Vector3D origin) { UNUSED(self); UNUSED(origin); return randomUnitVec3D(); }
#endif // !HITTABLE_H

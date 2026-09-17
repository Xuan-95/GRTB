#include "hittable.h"
#include "../core/common.h"
#include "../core/memory.h"
#include "../materials/material.h"
#include "../math/interval.h"
#include "../math/vector3d.h"
#include "aabb.h"
#include "hittable_list.h"
#include "quad.h"
#include "sphere.h"
#include <math.h>

void setFaceNormal(HitRecord *rec, Ray *r, Vector3D outward_normal) {
    rec->front_face = dot3D(r->direction, outward_normal) < 0;
    rec->normal     = rec->front_face ? outward_normal : scalarMultiply3D(-1.0, outward_normal);
}

HitRecord createHitRecord(Point3D p, Vector3D normal, double t, int front_face) {
    HitRecord hit_rec;
    hit_rec.p          = p;
    hit_rec.normal     = normal;
    hit_rec.t          = t;
    hit_rec.front_face = front_face;
    return hit_rec;
}

Hittable *constantMediumNew(Hittable *boundary, double density, Texture *texture) {
    Hittable *medium                             = ALLOCATE(Hittable, 1);
    medium->type                                 = HITTABLE_MEDIUM;
    medium->data.constant_medium.boundary        = boundary;
    medium->data.constant_medium.neg_inv_density = -1.0 / density;
    medium->data.constant_medium.phase_function  = createIsotropic(texture);
    medium->bbox                                 = boundary->bbox;
    return medium;
}

Hittable *createConstantMedium(Hittable *boundary, double density, Texture *texture) {
    return constantMediumNew(boundary, density, texture);
}
Hittable *createConstantMediumFromColor(Hittable *boundary, double density, Color color) {
    Texture *texture = createSolidColor(color);
    return constantMediumNew(boundary, density, texture);
}

int hitConstantMedium(Hittable *hittable, Ray *r, Interval ray_t, HitRecord *rec) {
    HitRecord hit_rec_1, hit_rec_2;

    Hittable *boundary = hittable->data.constant_medium.boundary;

    if (!hittableHit(boundary, r, createInterval(-INFINITY, INFINITY), &hit_rec_1)) {
        return 0;
    }
    if (!hittableHit(boundary, r, createInterval(hit_rec_1.t + 0.0001, INFINITY), &hit_rec_2)) {
        return 0;
    }

    if (hit_rec_1.t < ray_t.min)
        hit_rec_1.t = ray_t.min;
    if (hit_rec_2.t > ray_t.max)
        hit_rec_2.t = ray_t.max;

    if (hit_rec_1.t >= hit_rec_2.t)
        return 0;

    if (hit_rec_1.t < 0)
        hit_rec_1.t = 0;

    double ray_length               = length3D(r->direction);
    double distance_inside_boundary = (hit_rec_2.t - hit_rec_1.t) * ray_length;
    double hit_distance             = hittable->data.constant_medium.neg_inv_density * log(randomDouble(0, 1));
    if (hit_distance > distance_inside_boundary)
        return 0;

    rec->t          = hit_rec_1.t + hit_distance / ray_length;
    rec->p          = rayAt(*r, rec->t);
    rec->normal     = createVector3D(1, 0, 0);
    rec->front_face = 1;
    rec->mat        = hittable->data.constant_medium.phase_function;
    return 1;
}

double hittablePdfValue(Hittable *self, Point3D origin, Vector3D direction) {
    switch (self->type) {
    case HITTABLE_QUAD:
        return quadPdfValue(self, origin, direction);
    case HITTABLE_SPHERE:
        return spherePdfValue(self, origin, direction);
    case HITTABLE_HITLIST:
        return hittableListPdfValue(self, origin, direction);
    default:
        return 0.0;
    }
}

Vector3D hittableRandom(Hittable *self, Vector3D origin) {
    switch (self->type) {
    case HITTABLE_QUAD:
        return quadPdfRandom(self, origin);
    case HITTABLE_SPHERE:
        return spherePdfRandom(self, origin);
    case HITTABLE_HITLIST:
        return hittableListRandom(self, origin);
    default:
        return createVector3D(1, 0, 0);
    }
}

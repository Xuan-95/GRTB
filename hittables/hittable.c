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

Hittable *createTranslate(Hittable *object, Vector3D offset) {
    Hittable *translate              = ALLOCATE(Hittable, 1);
    translate->type                  = HITTABLE_TRANSLATE;
    translate->data.translate.object = object;
    translate->data.translate.offset = offset;
    translate->bbox                  = moveBbox(object->bbox, offset);

    return translate;
}

int hitTranslate(Hittable *hittable, Ray *r, Interval ray_t, HitRecord *rec) {

    Ray offset_ray = createRay(diff3D(r->origin, hittable->data.translate.offset), r->direction, r->time);
    if (!hittableHit(hittable->data.translate.object, &offset_ray, ray_t, rec)) {
        return 0;
    }
    rec->p = sum3D(rec->p, hittable->data.translate.offset);
    return 1;
}

Hittable *createRotateY(Hittable *object, double angle) {
    Hittable *rotate_y = ALLOCATE(Hittable, 1);
    rotate_y->type     = HITTABLE_ROTATE_Y;
    double radians     = degrees_to_radians(angle);

    rotate_y->data.rotate_y.sin_theta = sin(radians);
    rotate_y->data.rotate_y.cos_theta = cos(radians);
    rotate_y->data.rotate_y.object    = object;
    rotate_y->bbox                    = object->bbox;

    Point3D min = createVector3D(INFINITY, INFINITY, INFINITY);
    Point3D max = createVector3D(-INFINITY, -INFINITY, -INFINITY);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                // Select bbox corners
                double x = i * rotate_y->bbox.x.max + (1 - i) * rotate_y->bbox.x.min;
                double y = j * rotate_y->bbox.y.max + (1 - j) * rotate_y->bbox.y.min;
                double z = k * rotate_y->bbox.z.max + (1 - k) * rotate_y->bbox.z.min;

                // Apply rotation matrix
                double   newx = rotate_y->data.rotate_y.cos_theta * x + rotate_y->data.rotate_y.sin_theta * z;
                double   newz = -rotate_y->data.rotate_y.sin_theta * x + rotate_y->data.rotate_y.cos_theta * z;

                Vector3D tester = createVector3D(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min.e[c] = fmin(min.e[c], tester.e[c]);
                    max.e[c] = fmax(max.e[c], tester.e[c]);
                }
            }
        }
    }
    rotate_y->bbox = createAabbFromPoints(min, max);

    return (Hittable *)rotate_y;
}

int hitRotateY(Hittable *hittable, Ray *r, Interval ray_t, HitRecord *rec) {
    double  cos_theta = hittable->data.rotate_y.cos_theta;
    double  sin_theta = hittable->data.rotate_y.sin_theta;

    Point3D origin    = createVector3D((cos_theta * r->origin.x) - (sin_theta * r->origin.z), r->origin.y,
                                       (sin_theta * r->origin.x) + (cos_theta * r->origin.z));
    Point3D direction = createVector3D((cos_theta * r->direction.x) - (sin_theta * r->direction.z), r->direction.y,
                                       (sin_theta * r->direction.x) + (cos_theta * r->direction.z));
    Ray     rotated_y = createRay(origin, direction, r->time);

    if (!hittableHit(hittable->data.rotate_y.object, &rotated_y, ray_t, rec)) {
        return 0;
    }

    Point3D  p      = rec->p;
    Vector3D normal = rec->normal;
    rec->p      = createVector3D((cos_theta * p.x) + (sin_theta * p.z), p.y, (-sin_theta * p.x) + (cos_theta * p.z));
    rec->normal = createVector3D((cos_theta * normal.x) + (sin_theta * normal.z), normal.y,
                                 (-sin_theta * normal.x) + (cos_theta * normal.z));

    return 1;
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

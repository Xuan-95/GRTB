#include "hittable.h"
#include "aabb.h"
#include "common.h"
#include "interval.h"
#include "material.h"
#include "memory.h"
#include "vector3d.h"
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
    Translate *translate = ALLOCATE(Translate, 1);
    translate->object    = object;
    translate->offset    = offset;
    translate->base.hit  = hitTranslate;
    translate->base.bbox = moveBbox(object->bbox, offset);
    return (Hittable *)translate;
}

int hitTranslate(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec) {
    Translate *translate = (Translate *)self;

    Ray        offset_ray = createRay(diff3D(r->origin, translate->offset), r->direction, r->time);
    if (!translate->object->hit(translate->object, &offset_ray, ray_t, rec)) {
        return 0;
    }
    rec->p = sum3D(rec->p, translate->offset);
    return 1;
}

Hittable *createRotateY(Hittable *object, double angle) {
    RotateY *rotate_y = ALLOCATE(RotateY, 1);
    double   radians  = degrees_to_radians(angle);

    rotate_y->sin_theta = sin(radians);
    rotate_y->cos_theta = cos(radians);
    rotate_y->base.bbox = object->bbox;
    rotate_y->base.hit  = hitRotateY;
    rotate_y->object    = object;

    Point3D min = createVector3D(-INFINITY, -INFINITY, -INFINITY);
    Point3D max = createVector3D(INFINITY, INFINITY, INFINITY);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                // Select bbox corners
                double x = i * rotate_y->base.bbox.x.max + (1 - i) * rotate_y->base.bbox.x.min;
                double y = j * rotate_y->base.bbox.y.max + (1 - j) * rotate_y->base.bbox.y.min;
                double z = k * rotate_y->base.bbox.z.max + (1 - k) * rotate_y->base.bbox.z.min;

                // Apply rotation matrix
                double   newx = rotate_y->cos_theta * x + rotate_y->sin_theta * z;
                double   newz = -rotate_y->sin_theta * x + rotate_y->cos_theta * z;

                Vector3D tester = createVector3D(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min.e[c] = fmin(min.e[c], tester.e[c]);
                    max.e[c] = fmax(max.e[c], tester.e[c]);
                }
            }
        }
    }
    rotate_y->base.bbox = createAabbFromPoints(min, max);

    return (Hittable *)rotate_y;
}

int hitRotateY(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec) {
    RotateY *rotate_y = (RotateY *)self;

    Point3D  origin =
        createVector3D((rotate_y->cos_theta * r->origin.x) - (rotate_y->sin_theta * r->origin.z), r->origin.y,
                       (rotate_y->sin_theta * r->origin.x) + (rotate_y->cos_theta * r->origin.z));
    Point3D direction =
        createVector3D((rotate_y->cos_theta * r->direction.x) - (rotate_y->sin_theta * r->direction.z), r->direction.y,
                       (rotate_y->sin_theta * r->direction.x) + (rotate_y->cos_theta * r->direction.z));

    Ray rotated_y = createRay(origin, direction, r->time);

    if (!rotate_y->object->hit(rotate_y->object, &rotated_y, ray_t, rec)) {
        return 0;
    }

    rec->p = createVector3D((rotate_y->cos_theta * rec->p.x) + (rotate_y->sin_theta * rec->p.z), rec->p.y,
                            -(rotate_y->sin_theta * rec->p.x) + (rotate_y->cos_theta * rec->p.z));
    rec->normal =
        createVector3D((rotate_y->cos_theta * rec->normal.x) + (rotate_y->sin_theta * rec->normal.z), rec->normal.y,
                       -(rotate_y->sin_theta * rec->normal.x) + (rotate_y->cos_theta * rec->normal.z));

    return 1;
}

Hittable *createConstantMedium(Hittable *boundary, double density, Texture *texture) {
    ConstantMedium *medium = ALLOCATE(ConstantMedium, 1);

    medium->boundary        = boundary;
    medium->neg_inv_density = -1.0 / density;
    medium->phase_function  = createIsotropic(texture);
    medium->base.hit        = hitConstantMedium;
    medium->base.bbox       = boundary->bbox;
    return (Hittable *)medium;
}
Hittable *createConstantMediumFromColor(Hittable *boundary, double density, Color color) {
    ConstantMedium *medium = ALLOCATE(ConstantMedium, 1);

    medium->boundary        = boundary;
    medium->neg_inv_density = -1.0 / density;
    medium->phase_function  = createIsotropicFromColor(color);
    medium->base.hit        = hitConstantMedium;
    medium->base.bbox       = boundary->bbox;
    return (Hittable *)medium;
}
int hitConstantMedium(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec) {
    ConstantMedium *medium = (ConstantMedium *)self;
    HitRecord       hit_rec_1, hit_rec_2;

    if (!medium->boundary->hit(medium->boundary, r, createInterval(-INFINITY, INFINITY), &hit_rec_1)) {
        return 0;
    }
    if (!medium->boundary->hit(medium->boundary, r, createInterval(hit_rec_1.t + 0.0001, INFINITY), &hit_rec_2)) {
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
    double hit_distance             = medium->neg_inv_density * log(randomDouble(0, 1));
    if (hit_distance > distance_inside_boundary)
        return 0;

    rec->t          = hit_rec_1.t + hit_distance / ray_length;
    rec->p          = rayAt(*r, rec->t);
    rec->normal     = createVector3D(1, 0, 0);
    rec->front_face = 1;
    rec->mat        = medium->phase_function;
    return 1;
}

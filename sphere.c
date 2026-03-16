#include <math.h>

#include "aabb.h"
#include "interval.h"
#include "ray.h"
#include "sphere.h"
#include "vector3d.h"

Hittable *createSphere(Point3D center, double radius, Material *mat) {
    Sphere *s   = ALLOCATE(Sphere, 1);
    s->base.hit = hitSphere;

    Vector3D radius_vec = createVector3D(radius, radius, radius);
    Vector3D bbox_min   = diff3D(center, radius_vec);
    Vector3D bbox_max   = sum3D(center, radius_vec);
    s->base.bbox        = createAabbFromPoints(bbox_min, bbox_max);

    s->radius    = radius;
    s->center    = createRay(center, (Vector3D){0.0, 0.0, 0.0}, 0.0);
    s->mat       = mat;
    s->is_moving = 0;
    return (Hittable *)s;
}

Hittable *createMovingSphere(Point3D center_1, Point3D center_2, double radius, Material *mat) {
    Sphere *s    = ALLOCATE(Sphere, 1);
    s->base.hit  = hitSphere;
    s->radius    = radius;
    s->center    = createRay(center_1, diff3D(center_2, center_1), 0.0);
    s->mat       = mat;
    s->is_moving = 1;

    Vector3D radius_vec = createVector3D(radius, radius, radius);
    Point3D  center_t0  = rayAt(s->center, 0.0);
    Point3D  center_t1  = rayAt(s->center, 1.0);
    Aabb     box_t0     = createAabbFromPoints(diff3D(center_t0, radius_vec), sum3D(center_t0, radius_vec));
    Aabb     box_t1     = createAabbFromPoints(diff3D(center_t1, radius_vec), sum3D(center_t1, radius_vec));
    s->base.bbox        = unionAabb(box_t0, box_t1);

    return (Hittable *)s;
}

int hitSphere(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec) {

    Sphere *s = (Sphere *)self;

    // Evaluate determinant of the equation of the intersection between ray and
    // sphere
    Vector3D current_sphere = rayAt(s->center, r->time);
    Vector3D oc             = diff3D(r->origin, current_sphere);
    double   a              = dot3D(r->direction, r->direction);
    double   h              = -dot3D(r->direction, oc);
    double   c              = dot3D(oc, oc) - pow(s->radius, 2);
    double   discriminant   = pow(h, 2) - (a * c);

    if (discriminant < 0)
        return 0;

    // Evaluate if one root lies in the interval (ray_tmin, ray_tmax)
    double sqrtd = sqrt(discriminant);
    double root  = (h - sqrtd) / a;
    if (root <= ray_t.min || ray_t.max <= root) {
        root = (h + sqrtd) / a;
        if (root <= ray_t.min || ray_t.max <= root) {
            return 0;
        }
    }

    // There is an hit
    rec->t                  = root;
    rec->p                  = rayAt(*r, rec->t);
    rec->mat                = s->mat;
    Vector3D outward_normal = scalarDivide3D(diff3D(rec->p, current_sphere), s->radius);
    setFaceNormal(rec, r, outward_normal);
    return 1;
}

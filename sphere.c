#include <math.h>

#include "sphere.h"

Hittable *createSphere(Point3D center, double radius, Material *mat) {
    Sphere *s = ALLOCATE(Sphere, 1);
    s->base.hit = hitSphere;
    s->radius = radius;
    s->center = center;
    s->mat = mat;
    return (Hittable *)s;
}

int hitSphere(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec) {

    Sphere *s = (Sphere *)self;

    // Evaluate determinant of the equation of the intersection between ray and
    // sphere
    Vector3D oc = diff3D(r->origin, s->center);
    double a = dot3D(r->direction, r->direction);
    double h = -dot3D(r->direction, oc);
    double c = dot3D(oc, oc) - pow(s->radius, 2);
    double discriminant = pow(h, 2) - (a * c);

    if (discriminant < 0)
        return 0;

    // Evaluate if one root lies in the interval (ray_tmin, ray_tmax)
    double sqrtd = sqrt(discriminant);
    double root = (h - sqrtd) / a;
    if (root <= ray_t.min || ray_t.max <= root) {
        root = (h + sqrtd) / a;
        if (root <= ray_t.min || ray_t.max <= root) {
            return 0;
        }
    }

    // There is an hit
    rec->t = root;
    rec->p = rayAt(*r, rec->t);
    rec->mat = s->mat;
    Vector3D outward_normal =
        scalarDivide3D(diff3D(rec->p, s->center), s->radius);
    setFaceNormal(rec, r, outward_normal);
    return 1;
}

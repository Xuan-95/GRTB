#include <math.h>

#include "sphere.h"

int hitSphere(Hittable *self, Ray *r, double ray_tmin, double ray_tmax,
              HitRecord *rec) {

    Sphere *s = (Sphere *)self;

    // Evaluate determinant of the equation of the intersection between ray and
    // sphere
    Vector3D oc = diff3D(s->center, r->origin);
    double a = dot3D(r->direction, r->direction);
    double h = -dot3D(r->direction, oc);
    double c = dot3D(oc, oc) - pow(s->radius, 2);
    double discriminant = pow(h, 2) - (a * c);

    if (discriminant < 0)
        return 0;

    // Evaluate if one root lies in the interval (ray_tmin, ray_tmax)
    double sqrtd = sqrt(discriminant);
    double root = (h - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
        root = (h + sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            return 0;
        }
    }

    // There is an hit
    rec->t = root;
    rec->p = rayAt(*r, rec->t);
    Vector3D outward_normal =
        scalarDivide3D(diff3D(rec->p, s->center), s->radius);
    setFaceNormal(rec, r, outward_normal);
    return 1;
}

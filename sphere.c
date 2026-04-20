#include <math.h>

#include "aabb.h"
#include "hittable.h"
#include "interval.h"
#include "onb.h"
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
    // TODO: NotImplemented -> Default Implementation
    s->base.random   = hittableDefaultRandom;
    s->base.pdfValue = hittableDefaultPdfValue;

    s->radius    = radius;
    s->center    = createRay(center, (Vector3D){{{0.0, 0.0, 0.0}}}, 0.0);
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

    // TODO: NotImplemented -> Default Implementation
    s->base.random   = hittableDefaultRandom;
    s->base.pdfValue = hittableDefaultPdfValue;

    return (Hittable *)s;
}

void getSphereUV(Point3D p, double *u, double *v) {
    double theta = acos(-p.y);
    double phi   = atan2(-p.z, p.x) + PI;

    *u = phi / (2 * PI);
    *v = theta / PI;
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
    getSphereUV(outward_normal, &rec->u, &rec->v);
    return 1;
}

double spherePdf_Value(Hittable *self, Point3D origin, Vector3D direction) {
    Sphere   *sphere = (Sphere *)self;
    HitRecord hit_rec;
    Ray       ray = createRay(origin, direction, 0.0);
    if (!sphere->base.hit(self, &ray, createInterval(0.001, INFINITY), &hit_rec))
        return 0;

    double dist_squared  = lengthSquared3D(diff3D(rayAt(sphere->center, 0), origin));
    double cos_theta_max = sqrt(1 - sphere->radius * sphere->radius / dist_squared);
    double solid_angle   = 2 * PI * (1 - cos_theta_max);

    return 1 / solid_angle;
}

Vector3D spherePdf_Random(Hittable *self, Vector3D origin) {
    Sphere  *sphere           = (Sphere *)self;
    Vector3D direction        = diff3D(rayAt(sphere->center, 0.0), origin);
    double   distance_squared = lengthSquared3D(direction);
    Onb      uvw              = *createOnb(direction);
    return fromBasis(&uvw, randomToSphere(sphere->radius, distance_squared));
}

Vector3D randomToSphere(double radius, double distance_squared) {
    double r1 = randomDouble(0, 1);
    double r2 = randomDouble(0, 1);
    double z  = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

    double phi = 2 * PI * r1;
    double x   = cos(phi) * sqrt(1 - z * z);
    double y   = sin(phi) * sqrt(1 - z * z);

    return createVector3D(x, y, z);
}

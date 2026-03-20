#include "quad.h"
#include "common.h"
#include "hittable.h"
#include "interval.h"
#include "vector3d.h"

Hittable *createQuad(Point3D Q, Vector3D u, Vector3D v, Material *mat) {
    Quad *quad = ALLOCATE(Quad, 1);
    quad->Q    = Q;
    quad->u    = u;
    quad->v    = v;
    quad->mat  = mat;

    Vector3D n   = crossProduct3D(u, v);
    quad->normal = unitVector3D(n);
    quad->D      = dot3D(quad->normal, Q);
    quad->w      = scalarDivide3D(n, dot3D(n, n));

    Aabb bbox_diagonal_1 = createAabbFromPoints(Q, sum3D(Q, sum3D(u, v)));
    Aabb bbox_diagonal_2 = createAabbFromPoints(sum3D(Q, u), sum3D(Q, v));
    quad->base.bbox      = unionAabb(bbox_diagonal_1, bbox_diagonal_2);
    quad->base.hit       = hitQuad;
    return (Hittable *)quad;
}

int is_interior(double a, double b, HitRecord *rec) {
    Interval unit_interval = createInterval(0, 1);

    if (!contains(&unit_interval, a) || !contains(&unit_interval, b))
        return 0;

    rec->u = a;
    rec->v = b;
    return 1;
}

int hitQuad(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec) {
    Quad  *quad = (Quad *)self;

    double denom = dot3D(quad->normal, r->direction);
    if (fabs(denom) < 1e-8)
        return 0;

    double t = (quad->D - dot3D(quad->normal, r->origin)) / denom;
    if (!contains(&ray_t, t))
        return 0;

    Point3D  intersection        = rayAt(*r, t);
    Vector3D planar_hitpt_vector = diff3D(intersection, quad->Q);
    double   alpha               = dot3D(quad->w, crossProduct3D(planar_hitpt_vector, quad->v));
    double   beta                = dot3D(quad->w, crossProduct3D(quad->u, planar_hitpt_vector));
    if (!is_interior(alpha, beta, rec))
        return 0;

    rec->t   = t;
    rec->p   = intersection;
    rec->mat = quad->mat;
    setFaceNormal(rec, r, quad->normal);
    return 1;
}

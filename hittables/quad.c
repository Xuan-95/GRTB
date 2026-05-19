#include "quad.h"
#include "../core/common.h"
#include "../core/memory.h"
#include "../math/interval.h"
#include "../math/vector3d.h"
#include "hittable.h"
#include "hittable_list.h"

void buildQuadData(QuadData *quad, Point3D Q, Vector3D u, Vector3D v, Material *mat) {
    quad->Q   = Q;
    quad->u   = u;
    quad->v   = v;
    quad->mat = mat;

    Vector3D n   = crossProduct3D(u, v);
    quad->normal = unitVector3D(n);
    quad->D      = dot3D(quad->normal, Q);
    quad->w      = scalarDivide3D(n, dot3D(n, n));
    quad->area   = length3D(n);
}

Aabb calculateQuadBbox(Point3D Q, Vector3D u, Vector3D v) {
    Aabb bbox_diagonal_1 = createAabbFromPoints(Q, sum3D(Q, sum3D(u, v)));
    Aabb bbox_diagonal_2 = createAabbFromPoints(sum3D(Q, u), sum3D(Q, v));
    return unionAabb(bbox_diagonal_1, bbox_diagonal_2);
}

Hittable *createQuad(Point3D Q, Vector3D u, Vector3D v, Material *mat) {
    Hittable *hittable = ALLOCATE(Hittable, 1);
    hittable->type     = HITTABLE_QUAD;

    buildQuadData(&hittable->data.quad, Q, u, v, mat);
    hittable->bbox = calculateQuadBbox(Q, u, v);

    return hittable;
}

double quadPdfValue(Hittable *hittable, Point3D origin, Vector3D direction) {
    QuadData *quad = &hittable->data.quad;

    HitRecord rec;

    Ray       r     = createRay(origin, direction, 0);
    Interval  ray_t = createInterval(0.001, INFINITY);

    if (!hittableHit(hittable, &r, ray_t, &rec)) {
        return 0.0;
    }

    double distance_squared = (rec.t * rec.t) * lengthSquared3D(direction);
    double cosine           = fabs(dot3D(direction, rec.normal) / length3D(direction));

    if (cosine < 1e-9)
        return 0.0;

    return distance_squared / (cosine * quad->area);
}

Vector3D quadPdfRandom(Hittable *hittable, Vector3D origin) {
    QuadData *quad = &hittable->data.quad;
    Vector3D  p    = sum3D(quad->Q, (scalarMultiply3D(randomDouble(0, 1), quad->u)));
    p              = sum3D(p, scalarMultiply3D(randomDouble(0, 1), quad->v));
    return diff3D(p, origin);
}

int is_interior(double a, double b, HitRecord *rec) {
    Interval unit_interval = createInterval(0, 1);

    if (!contains(&unit_interval, a) || !contains(&unit_interval, b))
        return 0;

    rec->u = a;
    rec->v = b;
    return 1;
}

int hitQuad(Hittable *hittable, Ray *r, Interval ray_t, HitRecord *rec) {
    QuadData *quad = &hittable->data.quad;

    double    denom = dot3D(quad->normal, r->direction);
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

void addBox(HittableList *list, Point3D a, Point3D b, Material *mat) {
    Point3D  min = createVector3D(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    Point3D  max = createVector3D(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    Vector3D dx = createVector3D(max.x - min.x, 0, 0);
    Vector3D dy = createVector3D(0, max.y - min.y, 0);
    Vector3D dz = createVector3D(0, 0, max.z - min.z);
    addObject(list, createQuad(createVector3D(min.x, min.y, max.z), dx, dy, mat));
    addObject(list, createQuad(createVector3D(max.x, min.y, max.z), scalarMultiply3D(-1, dz), dy, mat));
    addObject(list, createQuad(createVector3D(max.x, min.y, min.z), scalarMultiply3D(-1, dx), dy, mat));
    addObject(list, createQuad(createVector3D(min.x, min.y, min.z), dz, dy, mat));
    addObject(list, createQuad(createVector3D(min.x, max.y, max.z), dx, scalarMultiply3D(-1, dz), mat));
    addObject(list, createQuad(createVector3D(min.x, min.y, min.z), dx, dz, mat));
}

#include "hittable.h"
#include "vector3d.h"

void setFaceNormal(HitRecord *rec, Ray *r, Vector3D outward_normal) {
    rec->front_face = dot3D(r->direction, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal
                                  : scalarMultiply3D(-1.0, outward_normal);
}

HitRecord createHitRecord(Point3D p, Vector3D normal, double t,
                          int front_face) {
    HitRecord hit_rec;
    hit_rec.p = p;
    hit_rec.normal = normal;
    hit_rec.t = t;
    hit_rec.front_face = front_face;
    return hit_rec;
}

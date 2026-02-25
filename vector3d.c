#include "vector3d.h"

Vector3D createVector3D(double x, double y, double z) {
    return (Vector3D){.x = x, .y = y, .z = z};
}
Vector3D negate3D(Vector3D v) {
    return (Vector3D){.x = -v.x, .y = -v.y, .z = -v.z};
}

double lengthSquared3D(Vector3D v) {
    return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

double length3D(Vector3D v) { return sqrt(lengthSquared3D(v)); }

void print3D(FILE *out, Vector3D v) { fprintf(out, "%f %f %f", v.x, v.y, v.z); }

Vector3D sum3D(Vector3D u, Vector3D v) {
    return (Vector3D){.x = u.x + v.x, .y = u.y + v.y, .z = u.z + v.z};
}

Vector3D diff3D(Vector3D u, Vector3D v) {
    return (Vector3D){.x = u.x - v.x, .y = u.y - v.y, .z = u.z - v.z};
}

Vector3D mul3D(Vector3D u, Vector3D v) {
    return (Vector3D){.x = u.x * v.x, .y = u.y * v.y, .z = u.z * v.z};
}

Vector3D scalarMultiply3D(double t, Vector3D v) {
    return (Vector3D){.x = t * v.x, .y = t * v.y, .z = t * v.z};
}

Vector3D scalarDivide3D(Vector3D v, double t) {
    return scalarMultiply3D(1.0 / t, v);
}

double dot3D(Vector3D u, Vector3D v) {
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

Vector3D crossProduct3D(Vector3D u, Vector3D v) {
    return (Vector3D){.x = (u.y * v.z) - (u.z * v.y),
                      .y = (u.z * v.x) - (u.x * v.z),
                      .z = (u.x * v.y) - (u.y * v.x)};
}

Vector3D unitVector3D(Vector3D v) { return scalarDivide3D(v, length3D(v)); }

#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>

#define RGB(r, g, b)                                                                                                   \
    (Color) { .x = (r), .y = (g), .z = (b) }

typedef struct {
    double x, y, z;
} Vector3D;

typedef Vector3D Point3D;
typedef Vector3D Color;

Vector3D createVector3D(double x, double y, double z);
Vector3D negate3D(Vector3D v);
double lengthSquared3D(Vector3D v);
double length3D(Vector3D v);
void print3D(FILE *out, Vector3D v);
Vector3D sum3D(Vector3D u, Vector3D v);
Vector3D diff3D(Vector3D u, Vector3D v);
Vector3D mul3D(Vector3D u, Vector3D v);
Vector3D scalarMultiply3D(double t, Vector3D v);
Vector3D scalarDivide3D(Vector3D v, double t);
double dot3D(Vector3D u, Vector3D v);
Vector3D crossProduct3D(Vector3D u, Vector3D v);
Vector3D unitVector3D(Vector3D v);
void writeColor(FILE *f, Color color);
Vector3D randomVec3D(double min, double max);
Vector3D randomUnitVec3D(void);
Vector3D randomOnHemisphere(Vector3D *normal);
int nearZero3D(Vector3D v);
Vector3D reflectVec3D(Vector3D v, Vector3D n);
Vector3D refractVec3D(Vector3D uv, Vector3D n, double etai_over_etat);
Vector3D random_in_unit_disk(void);

#endif

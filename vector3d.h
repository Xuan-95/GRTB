#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>

typedef struct {
    double x, y, z;
} Vector3D;

typedef Vector3D Point3D;

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

#endif

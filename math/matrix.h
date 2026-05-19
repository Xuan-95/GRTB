#ifndef MATRIX_H
#define MATRIX_H

#include "../core/common.h"
#include "vector3d.h"

// NOTE: This matrix structure is used only for rotations. Other operations will be implemented when needed
typedef struct {
    double m[3][3];
} Matrix3x3;

static inline Vector3D multiplyMatrixVector(Matrix3x3 mat, Vector3D v) {
    return createVector3D(mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z,
                          mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z,
                          mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z);
}

#endif // !MATRIX_H

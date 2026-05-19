#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "../core/common.h"
#include "../hittables/hittable.h"
#include "matrix.h"

void      translate(Hittable *objects, int n, Vector3D offset);
void      rotate(Hittable *objects, int n, Matrix3x3 rot_mat);
Matrix3x3 createRotationMatrixAxisAngle(Vector3D axis, double angle_rad);
void      rotateAroundPoint(Hittable *objects, size_t n, Matrix3x3 rot_mat, Point3D pivot);

#endif

#include "transformation.h"
#include "../hittables/quad.h"
#include "interval.h"
#include "matrix.h"
#include "vector3d.h"

void translate(Hittable *objects, int n, Vector3D offset) {
    for (int i = 0; i < n; i++) {
        switch (objects[i].type) {
        case HITTABLE_QUAD: {
            QuadData *quad  = &objects[i].data.quad;
            Point3D   new_Q = sum3D(quad->Q, offset);
            buildQuadData(quad, new_Q, quad->u, quad->v, quad->mat);
            objects[i].bbox = calculateQuadBbox(new_Q, quad->u, quad->v);
            break;
        }
        case HITTABLE_SPHERE: {
            objects[i].data.sphere.center.origin = sum3D(objects[i].data.sphere.center.origin, offset);
            objects[i].bbox.x                    = shiftInterval(objects[i].bbox.x, offset.x);
            objects[i].bbox.y                    = shiftInterval(objects[i].bbox.y, offset.y);
            objects[i].bbox.z                    = shiftInterval(objects[i].bbox.z, offset.z);
            break;
        }
        default:
            break;
        }
    }
}

void rotateAroundPoint(Hittable *objects, size_t n, Matrix3x3 rot_mat, Point3D pivot) {
    // Rotation matrix assume that the rotation is around origin
    // Translate to origin
    Vector3D to_origin = createVector3D(-pivot.x, -pivot.y, -pivot.z);
    translate(objects, n, to_origin);

    rotate(objects, n, rot_mat);

    Vector3D back_to_world = createVector3D(pivot.x, pivot.y, pivot.z);
    translate(objects, n, back_to_world);
}

// Rodrigues rotation
Matrix3x3 createRotationMatrixAxisAngle(Vector3D axis, double angle_rad) {
    axis = unitVector3D(axis);

    double    c = cos(angle_rad);
    double    s = sin(angle_rad);
    double    t = 1.0 - c;

    double    x = axis.x;
    double    y = axis.y;
    double    z = axis.z;

    Matrix3x3 mat;

    mat.m[0][0] = t * x * x + c;
    mat.m[0][1] = t * x * y - s * z;
    mat.m[0][2] = t * x * z + s * y;

    mat.m[1][0] = t * x * y + s * z;
    mat.m[1][1] = t * y * y + c;
    mat.m[1][2] = t * y * z - s * x;

    mat.m[2][0] = t * x * z - s * y;
    mat.m[2][1] = t * y * z + s * x;
    mat.m[2][2] = t * z * z + c;

    return mat;
}

void rotate(Hittable *objects, int n, Matrix3x3 rot_mat) {
    for (int i = 0; i < n; i++) {
        switch (objects[i].type) {
        case HITTABLE_QUAD: {
            QuadData *quad = &objects[i].data.quad;

            // Rotate vectors
            Point3D  new_Q = multiplyMatrixVector(rot_mat, quad->Q);
            Vector3D new_u = multiplyMatrixVector(rot_mat, quad->u);
            Vector3D new_v = multiplyMatrixVector(rot_mat, quad->v);

            buildQuadData(quad, new_Q, new_u, new_v, quad->mat);
            objects[i].bbox = calculateQuadBbox(new_Q, new_u, new_v);
            break;
        }
        case HITTABLE_SPHERE: {
            // Only the center need to rotate
            objects[i].data.sphere.center.origin = multiplyMatrixVector(rot_mat, objects[i].data.sphere.center.origin);

            Point3D ctr = objects[i].data.sphere.center.origin;
            double  r   = objects[i].data.sphere.radius;

            // New bbox change shape
            Point3D min_point = createVector3D(ctr.x - r, ctr.y - r, ctr.z - r);
            Point3D max_point = createVector3D(ctr.x + r, ctr.y + r, ctr.z + r);

            objects[i].bbox = createAabbFromPoints(min_point, max_point);
            break;
        }
        default:
            break;
        }
    }
}

#include "onb.h"

Onb *createOnb(Vector3D v) {
    Onb *onb     = ALLOCATE(Onb, 1);
    onb->axis[2] = unitVector3D(v);
    Vector3D a   = fabs(onb->axis[2].x) > 0.9 ? createVector3D(0, 1, 0) : createVector3D(1, 0, 0);
    onb->axis[1] = unitVector3D(crossProduct3D(onb->axis[2], a));
    onb->axis[0] = crossProduct3D(onb->axis[2], onb->axis[1]);
    return onb;
}

void initOnb(Onb *onb, Vector3D v) {
    onb->axis[2] = unitVector3D(v);
    Vector3D a   = fabs(onb->axis[2].x) > 0.9 ? createVector3D(0, 1, 0) : createVector3D(1, 0, 0);
    onb->axis[1] = unitVector3D(crossProduct3D(onb->axis[2], a));
    onb->axis[0] = crossProduct3D(onb->axis[2], onb->axis[1]);
}

Vector3D fromBasis(Onb *basis, Vector3D v) {
    return sum3D(sum3D(scalarMultiply3D(v.e[0], basis->axis[0]), scalarMultiply3D(v.e[1], basis->axis[1])),
                 scalarMultiply3D(v.e[2], basis->axis[2]));
}

#ifndef ONB_H
#define ONB_H

#include "common.h"

typedef struct {
    union {
        struct {
            Vector3D u, v, w;
        };
        Vector3D axis[3];
    };
} Onb;

Onb     *createOnb(Vector3D v);
void     initOnb(Onb *onb, Vector3D v);
Vector3D fromBasis(Onb *basis, Vector3D v);

#endif

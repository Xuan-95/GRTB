#ifndef PDF_H
#define PDF_H

#include "common.h"
#include "onb.h"

typedef struct Pdf Pdf;

struct Pdf {
    double (*value)(Pdf *base, Vector3D direction);
    Vector3D (*generate)(Pdf *base);
};

typedef struct {
    Pdf base;
} SpherePdf;

typedef struct {
    Pdf base;
    Onb onb;
} CosinePdf;

Pdf     *createSpherePdf(void);
void     initSpherePdf(SpherePdf *pdf);
double   spherePdfValue(Pdf *base, Vector3D direction);
Vector3D spherePdfgenerate(Pdf *base);

Pdf     *createCosinePdf(Vector3D vec);
void     initCosinePdf(CosinePdf *pdf, Vector3D vec);
double   cosinePdfValue(Pdf *base, Vector3D direction);
Vector3D cosinePdfgenerate(Pdf *base);

#endif

#ifndef PDF_H
#define PDF_H

#include "common.h"
#include "hittable.h"
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

typedef struct {
    Pdf       base;
    Hittable *objects;
    Point3D   origin;
} HittablePdf;

typedef struct {
    Pdf  base;
    Pdf *p[2];
} MixturePdf;

Pdf     *createSpherePdf(void);
void     initSpherePdf(SpherePdf *pdf);
double   spherePdfValue(Pdf *base, Vector3D direction);
Vector3D spherePdfGenerate(Pdf *base);

Pdf     *createCosinePdf(Vector3D vec);
void     initCosinePdf(CosinePdf *pdf, Vector3D vec);
double   cosinePdfValue(Pdf *base, Vector3D direction);
Vector3D cosinePdfGenerate(Pdf *base);

Pdf     *createHittablePdf(Hittable *objects, Point3D origin);
void     initHittablePdf(HittablePdf *pdf, Hittable *objects, Point3D origin);
double   hittablePdfValue(Pdf *base, Vector3D direction);
Vector3D hittablePdfGenerate(Pdf *base);

void     initMixturePdf(MixturePdf *pdf, Pdf *pdf_1, Pdf *pdf_2);
double   mixturePdfValue(Pdf *base, Vector3D direction);
Vector3D mixturePdfGenerate(Pdf *base);

#endif

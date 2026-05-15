#ifndef PDF_H
#define PDF_H

#include "../core/common.h"
#include "../hittables/hittable.h"
#include "../math/onb.h"

typedef enum { PDF_SPHERE, PDF_COSINE, PDF_HITTABLE, PDF_MIXTURE } PdfType;

typedef struct Pdf Pdf;

struct Pdf {
    PdfType type;
    union {
        struct {
            Onb onb;
        } cosine;

        struct {
            Hittable *objects;
            Point3D   origin;
        } hittable;

        struct {
            Pdf *p[2];
        } mixture;
    } data;
};

Pdf     *createSpherePdf(void);
void     initSpherePdf(Pdf *pdf);
double   _spherePdfValue(void);
Vector3D _spherePdfGenerate(void);

Pdf     *createCosinePdf(Vector3D vec);
void     initCosinePdf(Pdf *pdf, Vector3D vec);
double   _cosinePdfValue(Pdf *base, Vector3D direction);
Vector3D _cosinePdfGenerate(Pdf *base);

Pdf     *createHittablePdf(Hittable *objects, Point3D origin);
void     initHittablePdf(Pdf *pdf, Hittable *objects, Point3D origin);
double   _hittablePdfValue(Pdf *base, Vector3D direction);
Vector3D _hittablePdfGenerate(Pdf *base);

void     initMixturePdf(Pdf *pdf, Pdf *pdf_1, Pdf *pdf_2);
double   _mixturePdfValue(Pdf *base, Vector3D direction);
Vector3D _mixturePdfGenerate(Pdf *base);

//
static inline double pdfValue(Pdf *self, Vector3D direction) {
    switch (self->type) {
    case PDF_SPHERE:
        return _spherePdfValue();
    case PDF_COSINE:
        return _cosinePdfValue(self, direction);
    case PDF_HITTABLE:
        return _hittablePdfValue(self, direction);
    case PDF_MIXTURE:
        return _mixturePdfValue(self, direction);
    default:
        return 0.0;
    }
}

static inline Vector3D pdfGenerate(Pdf *self) {
    switch (self->type) {
    case PDF_SPHERE:
        return _spherePdfGenerate();
    case PDF_COSINE:
        return _cosinePdfGenerate(self);
    case PDF_HITTABLE:
        return _hittablePdfGenerate(self);
    case PDF_MIXTURE:
        return _mixturePdfGenerate(self);
    default:
        return createVector3D(1, 0, 0);
    }
}
#endif

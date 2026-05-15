#include "pdf.h"
#include "../core/memory.h"
#include "../math/onb.h"
#include "../math/vector3d.h"

Pdf *createSpherePdf(void) {
    Pdf *sphere_pdf  = ALLOCATE(Pdf, 1);
    sphere_pdf->type = PDF_SPHERE;
    return sphere_pdf;
}

void     initSpherePdf(Pdf *pdf) { pdf->type = PDF_SPHERE; }

double   _spherePdfValue(void) { return 1.0 / (4.0 * PI); }
Vector3D _spherePdfGenerate(void) { return randomUnitVec3D(); }

Pdf     *createCosinePdf(Vector3D vec) {
    Pdf *cosine_pdf = ALLOCATE(Pdf, 1);
    Onb  onb;
    initOnb(&onb, vec);
    cosine_pdf->type            = PDF_COSINE;
    cosine_pdf->data.cosine.onb = onb;
    return cosine_pdf;
}

void initCosinePdf(Pdf *pdf, Vector3D vec) {
    Onb onb;
    initOnb(&onb, vec);
    pdf->type            = PDF_COSINE;
    pdf->data.cosine.onb = onb;
}

double _cosinePdfValue(Pdf *base, Vector3D direction) {
    double cos_theta = dot3D(unitVector3D(direction), base->data.cosine.onb.w);
    return fmax(0, cos_theta / PI);
}

Vector3D _cosinePdfGenerate(Pdf *base) { return fromBasis(&base->data.cosine.onb, randomCosineDirection()); }

Pdf     *createHittablePdf(Hittable *objects, Point3D origin) {
    Pdf *hittable_pdf                   = ALLOCATE(Pdf, 1);
    hittable_pdf->type                  = PDF_HITTABLE;
    hittable_pdf->data.hittable.objects = objects;
    hittable_pdf->data.hittable.origin  = origin;
    return hittable_pdf;
}
void initHittablePdf(Pdf *pdf, Hittable *objects, Point3D origin) {
    pdf->type                  = PDF_HITTABLE;
    pdf->data.hittable.objects = objects;
    pdf->data.hittable.origin  = origin;
}

double _hittablePdfValue(Pdf *base, Vector3D direction) {
    return hittablePdfValue(base->data.hittable.objects, base->data.hittable.origin, direction);
}

Vector3D _hittablePdfGenerate(Pdf *base) {
    return hittableRandom(base->data.hittable.objects, base->data.hittable.origin);
}

void initMixturePdf(Pdf *pdf, Pdf *pdf_1, Pdf *pdf_2) {
    pdf->type              = PDF_MIXTURE;
    pdf->data.mixture.p[0] = pdf_1;
    pdf->data.mixture.p[1] = pdf_2;
}

double _mixturePdfValue(Pdf *self, Vector3D direction) {
    return 0.5 * pdfValue(self->data.mixture.p[0], direction) + 0.5 * pdfValue(self->data.mixture.p[1], direction);
}

Vector3D _mixturePdfGenerate(Pdf *self) {
    if (randomDouble(0, 1) < 0.5)
        return pdfGenerate(self->data.mixture.p[0]);
    return pdfGenerate(self->data.mixture.p[1]);
}

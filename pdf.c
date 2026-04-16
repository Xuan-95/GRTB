#include "pdf.h"
#include "memory.h"
#include "onb.h"
#include "vector3d.h"

Pdf *createSpherePdf(void) {
    SpherePdf *sphere_pdf     = ALLOCATE(SpherePdf, 1);
    sphere_pdf->base.generate = spherePdfGenerate;
    sphere_pdf->base.value    = spherePdfValue;
    return (Pdf *)sphere_pdf;
}

void initSpherePdf(SpherePdf *pdf) {
    pdf->base.generate = spherePdfGenerate;
    pdf->base.value    = spherePdfValue;
}

double   spherePdfValue(Pdf *base, Vector3D direction) { return 1.0 / (4.0 * PI); }
Vector3D spherePdfGenerate(Pdf *base) { return randomUnitVec3D(); }

Pdf     *createCosinePdf(Vector3D vec) {
    CosinePdf *cosine_pdf = ALLOCATE(CosinePdf, 1);
    Onb        onb;
    initOnb(&onb, vec);
    cosine_pdf->onb           = onb;
    cosine_pdf->base.generate = cosinePdfGenerate;
    cosine_pdf->base.value    = cosinePdfValue;
    return (Pdf *)cosine_pdf;
}

void initCosinePdf(CosinePdf *pdf, Vector3D vec) {
    Onb onb;
    initOnb(&onb, vec);
    pdf->onb           = onb;
    pdf->base.generate = cosinePdfGenerate;
    pdf->base.value    = cosinePdfValue;
}

double cosinePdfValue(Pdf *base, Vector3D direction) {
    CosinePdf *cosine_pdf = (CosinePdf *)base;
    double     cos_theta  = dot3D(unitVector3D(direction), cosine_pdf->onb.w);

    return fmax(0, cos_theta / PI);
}

Vector3D cosinePdfGenerate(Pdf *base) {
    CosinePdf *cosine_pdf = (CosinePdf *)base;
    return fromBasis(&cosine_pdf->onb, randomCosineDirection());
}

Pdf *createHittablePdf(Hittable *objects, Point3D origin) {
    HittablePdf *hittable_pdf   = ALLOCATE(HittablePdf, 1);
    hittable_pdf->objects       = objects;
    hittable_pdf->origin        = origin;
    hittable_pdf->base.generate = hittablePdfGenerate;
    hittable_pdf->base.value    = hittablePdfValue;
    return (Pdf *)hittable_pdf;
}
void initHittablePdf(HittablePdf *pdf, Hittable *objects, Point3D origin) {
    pdf->objects       = objects;
    pdf->origin        = origin;
    pdf->base.generate = hittablePdfGenerate;
    pdf->base.value    = hittablePdfValue;
}

double hittablePdfValue(Pdf *base, Vector3D direction) {
    HittablePdf *hittable_pdf = (HittablePdf *)base;
    return hittable_pdf->objects->pdfValue(hittable_pdf->objects, hittable_pdf->origin, direction);
}
Vector3D hittablePdfGenerate(Pdf *base) {
    HittablePdf *hittable_pdf = (HittablePdf *)base;
    return hittable_pdf->objects->random(hittable_pdf->objects, hittable_pdf->origin);
}

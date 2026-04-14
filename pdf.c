#include "pdf.h"
#include "memory.h"
#include "onb.h"
#include "vector3d.h"

Pdf *createSpherePdf(void) {
    SpherePdf *sphere_pdf     = ALLOCATE(SpherePdf, 1);
    sphere_pdf->base.generate = spherePdfgenerate;
    sphere_pdf->base.value    = spherePdfValue;
    return (Pdf *)sphere_pdf;
}

void initSpherePdf(SpherePdf *pdf) {
    pdf->base.generate = spherePdfgenerate;
    pdf->base.value    = spherePdfValue;
}

double   spherePdfValue(Pdf *base, Vector3D direction) { return 1 / (4 * PI); }
Vector3D spherePdfgenerate(Pdf *base) { return randomUnitVec3D(); }

Pdf     *createCosinePdf(Vector3D vec) {
    CosinePdf *cosine_pdf = ALLOCATE(CosinePdf, 1);
    Onb        onb;
    initOnb(&onb, vec);
    cosine_pdf->onb           = onb;
    cosine_pdf->base.generate = cosinePdfgenerate;
    cosine_pdf->base.value    = cosinePdfValue;
    return (Pdf *)cosine_pdf;
}

void initCosinePdf(CosinePdf *pdf, Vector3D vec) {
    Onb onb;
    initOnb(&onb, vec);
    pdf->onb           = onb;
    pdf->base.generate = cosinePdfgenerate;
    pdf->base.value    = cosinePdfValue;
}

double cosinePdfValue(Pdf *base, Vector3D direction) {
    CosinePdf *cosine_pdf = (CosinePdf *)base;
    double     cos_theta  = dot3D(unitVector3D(direction), cosine_pdf->onb.w);

    return fmax(0, cos_theta / PI);
}

Vector3D cosinePdfgenerate(Pdf *base) {
    CosinePdf *cosine_pdf = (CosinePdf *)base;
    return fromBasis(&cosine_pdf->onb, randomCosineDirection());
}

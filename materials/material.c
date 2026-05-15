#include "material.h"
#include "../core/common.h"
#include "../core/memory.h"
#include "../hittables/aabb.h"
#include "../math/vector3d.h"
#include "../rendering/pdf.h"
#include "../textures/texture.h"
#include <math.h>

static inline Material *lambertianNew(Texture *texture) {
    Material *lambertian                = ALLOCATE(Material, 1);
    lambertian->type                    = MAT_LAMBERTIAN;
    lambertian->data.lambertian.texture = texture;
    return lambertian;
}

Material *createLambertian(Color albedo) { return lambertianNew(createSolidColor(albedo)); }
Material *createLambertianFromTexture(Texture *texture) { return lambertianNew(texture); }

int       lambertianScatter(Material *mat, HitRecord *hit_rec, ScatterRecord *scatter_rec) {
    // NOTE: Many small allocations. Consider using a PdfBuffer as input for scatter functions
    Pdf *cosine_pdf = createCosinePdf(hit_rec->normal);

    scatter_rec->attenuation =
        mat->data.lambertian.texture->value(mat->data.lambertian.texture, hit_rec->u, hit_rec->v, hit_rec->p);
    scatter_rec->pdf      = cosine_pdf;
    scatter_rec->skip_pdf = false;
    return 1;
}

double lambertianScatteringPdf(Material *self, Ray *ray_in, HitRecord *hit_rec, Ray *scattered) {
    UNUSED(self);
    UNUSED(ray_in);
    double cos_theta = dot3D(hit_rec->normal, unitVector3D(scattered->direction));
    return cos_theta < 0 ? 0 : cos_theta / PI;
}

Material *createMetal(Color albedo, double fuzz) {
    Material *metal          = ALLOCATE(Material, 1);
    metal->type              = MAT_METAL;
    metal->data.metal.albedo = albedo;
    metal->data.metal.fuzz   = fuzz;
    return metal;
}

int metalScatter(Material *mat, Ray *ray_in, HitRecord *hit_rec, ScatterRecord *scatter_rec) {

    // Metal scatter apply a "perfect" reflection, perturbed by the fuzz
    Vector3D reflected = reflectVec3D(ray_in->direction, hit_rec->normal);
    reflected          = sum3D(unitVector3D(reflected), (scalarMultiply3D(mat->data.metal.fuzz, randomUnitVec3D())));

    scatter_rec->attenuation  = mat->data.metal.albedo;
    scatter_rec->pdf          = NULL;
    scatter_rec->skip_pdf     = true;
    scatter_rec->skip_pdf_ray = createRay(hit_rec->p, reflected, ray_in->time);
    return dot3D(reflected, hit_rec->normal) > 0;
}

Material *createDielectric(double refraction_index) {
    Material *dielectric                         = ALLOCATE(Material, 1);
    dielectric->type                             = MAT_DIELECTRIC;
    dielectric->data.dielectric.refraction_index = refraction_index;
    return dielectric;
}

int dielectricScatter(Material *mat, Ray *ray_in, HitRecord *hit_rec, ScatterRecord *scatter_rec) {
    scatter_rec->attenuation = (Color){.x = 1.0, .y = 1.0, .z = 1.0};
    scatter_rec->skip_pdf    = true;
    scatter_rec->pdf         = NULL;

    double ri =
        hit_rec->front_face ? (1.0 / mat->data.dielectric.refraction_index) : mat->data.dielectric.refraction_index;
    Vector3D unit_direction = unitVector3D(ray_in->direction);
    double   cos_theta      = dot3D(scalarMultiply3D(-1.0, unit_direction), hit_rec->normal);
    cos_theta               = fmin(cos_theta, 1.0);
    double sin_theta        = sqrt(1.0 - cos_theta * cos_theta);

    // Trigger total internal reflection if sin_theta is greater than the critical angle
    int      cannot_refract = ri * sin_theta > 1.0;

    Vector3D direction;
    // Fresnel gives the probability of reflection vs refraction.
    // Monte Carlo: pick one stochastically instead of splitting into two rays.
    if (cannot_refract || dielectricReflectance(ri, cos_theta) > randomDouble(0.0, 1.0)) {
        direction = reflectVec3D(unit_direction, hit_rec->normal); // Total internal reflection or Fresnel reflection
    } else {
        direction = refractVec3D(unit_direction, hit_rec->normal, ri); // refraction (Snell's law)
    }

    scatter_rec->skip_pdf_ray = createRay(hit_rec->p, direction, ray_in->time);
    return 1;
}

double dielectricReflectance(double refraction_index, double cosine) {
    // Schlick's approximation
    double r0 = (1.0 - refraction_index) / (1.00 + refraction_index);
    r0        = r0 * r0;
    return r0 + (1.0 - r0) * pow(1.0 - cosine, 5);
}

static inline Material *diffuseLightNew(Texture *tex) {
    Material *light                   = ALLOCATE(Material, 1);
    light->type                       = MAT_DIFFUSE_LIGHT;
    light->data.diffuse_light.texture = tex;
    return light;
}

Material *createDiffuseLight(Texture *tex) { return diffuseLightNew(tex); }
Material *createDiffuseLightFromColor(Color emit) { return diffuseLightNew(createSolidColor(emit)); }

Color     diffuseLightEmitted(Material *mat, HitRecord *hit_rec, double u, double v, Point3D p) {
    if (!hit_rec->front_face) {
        return RGB(0.0, 0.0, 0.0);
    }
    return mat->data.diffuse_light.texture->value(mat->data.diffuse_light.texture, u, v, p);
}

static inline Material *isotropicNew(Texture *tex) {
    Material *isotropic               = ALLOCATE(Material, 1);
    isotropic->type                   = MAT_ISOTROPIC;
    isotropic->data.isotropic.texture = tex;
    return isotropic;
}

Material *createIsotropic(Texture *tex) { return isotropicNew(tex); }
Material *createIsotropicFromColor(Color albedo) { return isotropicNew(createSolidColor(albedo)); }

int       isotropicScatter(Material *mat, HitRecord *hit_rec, ScatterRecord *scatter_rec) {

    // NOTE: Many small allocations. Consider using a PdfBuffer as input for scatter functions
    Pdf *sphere_pdf = createSpherePdf();

    scatter_rec->attenuation =
        mat->data.isotropic.texture->value(mat->data.isotropic.texture, hit_rec->u, hit_rec->v, hit_rec->p);
    scatter_rec->pdf      = sphere_pdf;
    scatter_rec->skip_pdf = false;
    return 1;
}

double isotropicScatteringPdf(void) { return 1 / (4 * PI); }

int    materialScatter(Material *mat, Ray *ray_in, HitRecord *hit_rec, ScatterRecord *scatter_rec) {
    switch (mat->type) {
    case MAT_LAMBERTIAN:
        return lambertianScatter(mat, hit_rec, scatter_rec);
    case MAT_METAL:
        return metalScatter(mat, ray_in, hit_rec, scatter_rec);
    case MAT_DIELECTRIC:
        return dielectricScatter(mat, ray_in, hit_rec, scatter_rec);
    case MAT_ISOTROPIC:
        return isotropicScatter(mat, hit_rec, scatter_rec);
    default:
        return false;
    }
}
Color materialEmitted(Material *mat, HitRecord *hit_rec, double u, double v, Point3D p) {
    switch (mat->type) {
    case MAT_DIFFUSE_LIGHT:
        return diffuseLightEmitted(mat, hit_rec, u, v, p);
    default:
        return RGB(0.0, 0.0, 0.0);
    }
}

double materialScatteringPdf(Material *mat, Ray *ray_in, HitRecord *hit_rec, Ray *scattered) {
    switch (mat->type) {
    case MAT_LAMBERTIAN:
        return lambertianScatteringPdf(mat, ray_in, hit_rec, scattered);
    case MAT_ISOTROPIC:
        return isotropicScatteringPdf();
    default:
        return 0.0;
    }
}

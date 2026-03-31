#include "material.h"
#include "aabb.h"
#include "common.h"
#include "memory.h"
#include "texture.h"
#include "vector3d.h"
#include <math.h>

Material *createLambertian(Color albedo) {
    Lambertian *lambertian   = ALLOCATE(Lambertian, 1);
    lambertian->base.scatter = lambertianScatter;
    lambertian->texture      = createSolidColor(albedo);
    return (Material *)lambertian;
}

Material *createLambertianFromTexture(Texture *texture) {
    Lambertian *lambertian   = ALLOCATE(Lambertian, 1);
    lambertian->texture      = texture;
    lambertian->base.scatter = lambertianScatter;
    return (Material *)lambertian;
}

int lambertianScatter(Material *self, Ray *ray_in, HitRecord *hit_rec, Color *attenuation, Ray *scattered) {
    Lambertian *lambertian        = (Lambertian *)self;
    Vector3D    scatter_direction = sum3D(hit_rec->normal, randomUnitVec3D());
    if (nearZero3D(scatter_direction)) {
        scatter_direction = hit_rec->normal;
    }
    *scattered   = createRay(hit_rec->p, scatter_direction, ray_in->time);
    *attenuation = lambertian->texture->value(lambertian->texture, hit_rec->u, hit_rec->v, hit_rec->p);
    return 1;
}

Material *createMetal(Color albedo, double fuzz) {
    Metal *metal        = ALLOCATE(Metal, 1);
    metal->albedo       = albedo;
    metal->fuzz         = fuzz;
    metal->base.scatter = metalScatter;
    return (Material *)metal;
}

int metalScatter(Material *self, Ray *ray_in, HitRecord *hit_rec, Color *attenuation, Ray *scattered) {
    Metal   *metal = (Metal *)self;

    Vector3D reflected = reflectVec3D(ray_in->direction, hit_rec->normal);
    reflected          = sum3D(unitVector3D(reflected), (scalarMultiply3D(metal->fuzz, randomUnitVec3D())));
    *scattered         = createRay(hit_rec->p, reflected, ray_in->time);
    *attenuation       = metal->albedo;
    return 1;
}

Material *createDielectric(double refraction_index) {
    Dielectric *dielectric       = ALLOCATE(Dielectric, 1);
    dielectric->base.scatter     = dielectricScatter;
    dielectric->refraction_index = refraction_index;
    return (Material *)dielectric;
}

int dielectricScatter(Material *self, Ray *ray_in, HitRecord *hit_rec, Color *attenuation, Ray *scattered) {
    Dielectric *dielectric  = (Dielectric *)self;
    *attenuation            = (Color){.x = 1.0, .y = 1.0, .z = 1.0};
    double   ri             = hit_rec->front_face ? (1.0 / dielectric->refraction_index) : dielectric->refraction_index;
    Vector3D unit_direction = unitVector3D(ray_in->direction);
    double   cos_theta      = dot3D(scalarMultiply3D(-1.0, unit_direction), hit_rec->normal);
    cos_theta               = fmin(cos_theta, 1.0);
    double   sin_theta      = sqrt(1.0 - cos_theta * cos_theta);
    int      cannot_refract = ri * sin_theta > 1.0;
    Vector3D direction;
    if (cannot_refract || dielectricReflectance(cos_theta, ri) > randomDouble(0.0, 1.0)) {
        direction = reflectVec3D(unit_direction, hit_rec->normal);
    } else {
        direction = refractVec3D(unit_direction, hit_rec->normal, ri);
    }

    *scattered = createRay(hit_rec->p, direction, ray_in->time);
    return 1;
}

double dielectricReflectance(double refraction_index, double cosine) {
    // Schlick's approximation
    double r0 = (1.0 - refraction_index) / (1 + refraction_index);
    r0        = r0 * r0;
    return r0 + (1.0 - r0) * pow(1.0 - cosine, 5);
}

Material *createDiffuseLight(Texture *tex) {
    DiffuseLight *light = ALLOCATE(DiffuseLight, 1);
    light->tex          = tex;
    light->base.scatter = NULL;
    light->base.emitted = diffuseLightEmitted;
    return (Material *)light;
}

Material *createDiffuseLightFromColor(Color emit) {
    Texture *tex = createSolidColor(emit);
    return createDiffuseLight(tex);
}

Color diffuseLightEmitted(Material *self, double u, double v, Point3D p) {
    DiffuseLight *light = (DiffuseLight *)self;
    return light->tex->value(light->tex, u, v, p);
}

Material *createIsotropic(Texture *tex) {
    Isotropic *isotropic = ALLOCATE(Isotropic, 1);

    isotropic->tex          = tex;
    isotropic->base.scatter = IsotropicScatter;
    return (Material *)isotropic;
}
Material *createIsotropicFromColor(Color albedo) {
    Texture *texture = createSolidColor(albedo);
    return createIsotropic(texture);
}
int IsotropicScatter(Material *self, Ray *ray_in, HitRecord *hit_rec, Color *attenuation, Ray *scattered) {
    Isotropic *isotropic = (Isotropic *)self;
    *scattered           = createRay(hit_rec->p, randomUnitVec3D(), ray_in->time);
    *attenuation         = isotropic->tex->value(isotropic->tex, hit_rec->u, hit_rec->v, hit_rec->p);
    return 1;
}

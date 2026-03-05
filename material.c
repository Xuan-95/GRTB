#include "material.h"
#include "memory.h"
#include "vector3d.h"
#include <math.h>

Material *createLambertian(Color albedo) {
    Lambertian *lambertian = ALLOCATE(Lambertian, 1);
    lambertian->base.scatter = lambertianScatter;
    lambertian->albedo = albedo;
    return (Material *)lambertian;
}

int lambertianScatter(Material *self, Ray *ray_in, HitRecord *hit_rec,
                      Color *attenuation, Ray *scattered) {
    Lambertian *lambertian = (Lambertian *)self;
    Vector3D scatter_direction = sum3D(hit_rec->normal, randomUnitVec3D());
    if (nearZero3D(scatter_direction)) {
        scatter_direction = hit_rec->normal;
    }
    *scattered = createRay(hit_rec->p, scatter_direction);
    *attenuation = lambertian->albedo;
    return 1;
}

Material *createMetal(Color albedo, double fuzz) {
    Metal *metal = ALLOCATE(Metal, 1);
    metal->albedo = albedo;
    metal->fuzz = fuzz;
    metal->base.scatter = metalScatter;
    return (Material *)metal;
}

int metalScatter(Material *self, Ray *ray_in, HitRecord *hit_rec,
                 Color *attenuation, Ray *scattered) {
    Metal *metal = (Metal *)self;

    Vector3D reflected = reflectVec3D(ray_in->direction, hit_rec->normal);
    reflected = sum3D(unitVector3D(reflected),
                      (scalarMultiply3D(metal->fuzz, randomUnitVec3D())));
    *scattered = createRay(hit_rec->p, reflected);
    *attenuation = metal->albedo;
    return 1;
}

Material *createDielectric(double refraction_index) {
    Dielectric *dielectric = ALLOCATE(Dielectric, 1);
    dielectric->base.scatter = dielectricScatter;
    dielectric->refraction_index = refraction_index;
    return (Material *)dielectric;
}

int dielectricScatter(Material *self, Ray *ray_in, HitRecord *hit_rec,
                      Color *attenuation, Ray *scattered) {
    Dielectric *dielectric = (Dielectric *)self;
    *attenuation = (Color){.x = 1.0, .y = 1.0, .z = 1.0};
    double ri = hit_rec->front_face ? (1.0 / dielectric->refraction_index)
                                    : dielectric->refraction_index;
    Vector3D unit_direction = unitVector3D(ray_in->direction);
    double cos_theta =
        dot3D(scalarMultiply3D(-1.0, unit_direction), hit_rec->normal);
    cos_theta = fmin(cos_theta, 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    int cannot_refract = ri * sin_theta > 1.0;
    Vector3D direction;
    if (cannot_refract) {
        direction = reflectVec3D(unit_direction, hit_rec->normal);
    } else {
        direction = refractVec3D(unit_direction, hit_rec->normal, ri);
    }

    *scattered = createRay(hit_rec->p, direction);
    return 1;
}


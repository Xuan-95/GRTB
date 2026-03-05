#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "hittable.h"

typedef struct Material Material;

struct Material {
    int (*scatter)(Material *self, Ray *ray_in, HitRecord *hit_rec,
                   Color *attenuation, Ray *scattered);
};

typedef struct {
    Material base;
    Color albedo;
} Lambertian;

typedef struct {
    Material base;
    Color albedo;
    double fuzz;
} Metal;

typedef struct {
    Material base;
    double refraction_index;
} Dielectric;

Material *createLambertian(Color albedo);
int lambertianScatter(Material *self, Ray *ray_in, HitRecord *hit_rec,
                      Color *attenuation, Ray *scattered);

Material *createMetal(Color albedo, double fuzz);
int metalScatter(Material *self, Ray *ray_in, HitRecord *hit_rec,
                 Color *attenuation, Ray *scattered);

Material *createDielectric(double refraction_index);
int dielectricScatter(Material *self, Ray *ray_in, HitRecord *hit_rec,
                      Color *attenuation, Ray *scattered);
double dielectricReflectance(double refraction_index, double cosine);

#endif // !MATERIAL_H

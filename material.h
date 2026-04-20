#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "hittable.h"
#include "pdf.h"
#include "texture.h"

typedef struct {
    Color attenuation;
    Pdf  *pdf;
    bool  skip_pdf;
    Ray   skip_pdf_ray;
} ScatterRecord;

typedef struct Material Material;

struct Material {
    int (*scatter)(Material *self, Ray *ray_in, HitRecord *hit_rec, ScatterRecord *scatter_rec);
    Color (*emitted)(Material *self, HitRecord *hit_rec, double u, double v, Point3D p);
    double (*scatteringPdf)(Material *self, Ray *ray_in, HitRecord *hit_rec, Ray *scattered);
};

typedef struct {
    Material base;
    Texture *texture;
} Lambertian;

typedef struct {
    Material base;
    Color    albedo;
    double   fuzz;
} Metal;

typedef struct {
    Material base;
    double   refraction_index;
} Dielectric;

typedef struct {
    Material base;
    Texture *tex;
} DiffuseLight;

typedef struct {
    Material base;
    Texture *tex;
} Isotropic;

Material *createLambertian(Color albedo);
Material *createLambertianFromTexture(Texture *texture);
int       lambertianScatter(Material *self, Ray *ray_in, HitRecord *hit_rec, ScatterRecord *scatter_rec);
double    lambertianScatteringPdf(Material *self, Ray *ray_in, HitRecord *hit_rec, Ray *scattered);

Material *createMetal(Color albedo, double fuzz);
int       metalScatter(Material *self, Ray *ray_in, HitRecord *hit_rec, ScatterRecord *scatter_rec);

Material *createDielectric(double refraction_index);
int       dielectricScatter(Material *self, Ray *ray_in, HitRecord *hit_rec, ScatterRecord *scatter_rec);
double    dielectricReflectance(double refraction_index, double cosine);

Material *createDiffuseLight(Texture *tex);
Material *createDiffuseLightFromColor(Color emit);
Color     diffuseLightEmitted(Material *self, HitRecord *hit_rec, double u, double v, Point3D p);

Material *createIsotropic(Texture *tex);
Material *createIsotropicFromColor(Color albedo);
int       IsotropicScatter(Material *self, Ray *ray_in, HitRecord *hit_rec, ScatterRecord *scatter_rec);
double    isotropicScatteringPdf(Material *self, Ray *ray_in, HitRecord *hit_rec, Ray *scattered);

#endif
// !MATERIAL_H

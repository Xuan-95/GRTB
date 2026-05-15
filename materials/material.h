#ifndef MATERIAL_H
#define MATERIAL_H

#include "../core/common.h"
#include "../hittables/hittable.h"
#include "../rendering/pdf.h"
#include "../textures/texture.h"

typedef struct Material Material;

typedef struct {
    Color attenuation;
    Pdf  *pdf;
    bool  skip_pdf;
    Ray   skip_pdf_ray;
} ScatterRecord;

typedef enum {
    MAT_LAMBERTIAN,
    MAT_METAL,
    MAT_DIELECTRIC,
    MAT_DIFFUSE_LIGHT,
    MAT_ISOTROPIC,
} MaterialType;

struct Material {
    MaterialType type;
    union {
        struct {
            Texture *texture;
        } lambertian;
        struct {
            Color  albedo;
            double fuzz;
        } metal;
        struct {
            double refraction_index;
        } dielectric;
        struct {
            Texture *texture;
        } diffuse_light;
        struct {
            Texture *texture;
        } isotropic;
    } data;
};

int       materialScatter(Material *self, Ray *ray_in, HitRecord *hit_rec, ScatterRecord *scatter_rec);
Color     materialEmitted(Material *self, HitRecord *hit_rec, double u, double v, Point3D p);
double    materialScatteringPdf(Material *self, Ray *ray_in, HitRecord *hit_rec, Ray *scattered);

Material *createLambertian(Color albedo);
Material *createLambertianFromTexture(Texture *texture);
int       lambertianScatter(Material *self, HitRecord *hit_rec, ScatterRecord *scatter_rec);
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
int       isotropicScatter(Material *self, HitRecord *hit_rec, ScatterRecord *scatter_rec);
double    isotropicScatteringPdf(void);

#endif
// !MATERIAL_H

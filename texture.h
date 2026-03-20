#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"
#include "grbt_stb_image.h"
#include "perlin.h"

typedef struct Texture Texture;

struct Texture {
    Color (*value)(Texture *self, double u, double v, const Point3D p);
};

typedef struct {
    Texture base;
    Color   albedo;
} SolidColor;

typedef struct {
    Texture  base;
    double   inv_scale;
    Texture *even;
    Texture *odd;
} CheckerTexture;

typedef struct {
    Texture    base;
    GRTBImage *image;
} ImageTexture;

typedef struct {
    Texture base;
    Perlin *noise;
    double  scale;
} PerlinTexture;

Texture *createSolidColor(Color albedo);
Texture *createSolidColorRGB(double r, double g, double b);
Color    solidColorValue(Texture *self, double u, double v, const Point3D p);

Texture *createCheckerTexture(double scale, Texture *even, Texture *odd);
Texture *createCheckerTextureRGB(double r, Color color_1, Color color_2);
Color    checkerTextureValue(Texture *self, double u, double v, const Point3D p);

Texture *createImageTexture(const char *filename);
Color    imageTextureValue(Texture *self, double u, double v, const Point3D p);

Texture *createPerlinTexture(double scale);
Color    perlinTextureValue(Texture *self, double u, double v, const Point3D p);

#endif // !TEXTURE_H

#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

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

Texture *createSolidColor(Color albedo);
Texture *createSolidColorRGB(double r, double g, double b);
Color    solidColorValue(Texture *self, double u, double v, const Point3D p);

Texture *createCheckerTexture(double scale, Texture *even, Texture *odd);
Texture *createCheckerTextureRGB(double r, Color color_1, Color color_2);
Color    checkerTextureValue(Texture *self, double u, double v, const Point3D p);

#endif // !TEXTURE_H

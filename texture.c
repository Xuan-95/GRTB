#include "texture.h"
#include "memory.h"
#include <math.h>

Texture *createSolidColor(Color albedo) {
    SolidColor *solid_color = ALLOCATE(SolidColor, 1);
    solid_color->albedo     = albedo;
    solid_color->base.value = solidColorValue;
    return (Texture *)solid_color;
}

Texture *createSolidColorRGB(double r, double g, double b) {
    Color albedo = RGB(r, g, b);
    return createSolidColor(albedo);
}

Color solidColorValue(Texture *self, double u, double v, const Point3D p) {
    SolidColor *solid_color = (SolidColor *)self;
    return solid_color->albedo;
}

Texture *createCheckerTexture(double scale, Texture *even, Texture *odd) {
    CheckerTexture *checker_texture = ALLOCATE(CheckerTexture, 1);
    checker_texture->inv_scale      = 1.0 / scale;
    checker_texture->even           = even;
    checker_texture->odd            = odd;
    checker_texture->base.value     = checkerTextureValue;
    return (Texture *)checker_texture;
}

Texture *createCheckerTextureRGB(double scale, Color color_1, Color color_2) {
    Texture *even = createSolidColor(color_1);
    Texture *odd  = createSolidColor(color_2);
    return createCheckerTexture(scale, even, odd);
}
Color checkerTextureValue(Texture *self, double u, double v, const Point3D p) {
    CheckerTexture *checker_texture = (CheckerTexture *)self;

    int             xInteger = (int)(floor(checker_texture->inv_scale * p.x));
    int             yInteger = (int)(floor(checker_texture->inv_scale * p.y));
    int             zInteger = (int)(floor(checker_texture->inv_scale * p.z));

    int             isEven = (xInteger + yInteger + zInteger) % 2 == 0;
    return isEven ? checker_texture->even->value(checker_texture->even, u, v, p)
                  : checker_texture->odd->value(checker_texture->odd, u, v, p);
}

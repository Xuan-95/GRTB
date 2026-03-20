#include "texture.h"
#include "common.h"
#include "interval.h"
#include "memory.h"
#include "perlin.h"
#include "vector3d.h"
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

Texture *createImageTexture(const char *filename) {
    ImageTexture *image_texture = ALLOCATE(ImageTexture, 1);
    image_texture->image        = createImage(filename);
    image_texture->base.value   = imageTextureValue;
    return (Texture *)image_texture;
}

Color imageTextureValue(Texture *self, double u, double v, const Point3D p) {
    ImageTexture *image_texture = (ImageTexture *)self;
    GRTBImage    *image         = image_texture->image;
    if (imageHeight(image) == 0)
        return createVector3D(0.0, 1.0, 1.0);

    Interval value_span = createInterval(0, 1);

    u = clampInterval(&value_span, u);
    v = 1.0 - clampInterval(&value_span, v);

    int                  i           = (int)(u * imageWidth(image));
    int                  j           = (int)(v * imageHeight(image));
    const unsigned char *pixel       = pixelData(image, i, j);
    double               color_scale = 1.0 / 255.0;

    return createVector3D(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}

Texture *createPerlinTexture(void) {
    PerlinTexture *perlin_texture = ALLOCATE(PerlinTexture, 1);
    perlin_texture->base.value    = perlinTextureValue;
    perlin_texture->noise         = createPerlin();
    return (Texture *)perlin_texture;
}
Color perlinTextureValue(Texture *self, double u, double v, const Point3D p) {
    PerlinTexture *perlin_texture = (PerlinTexture *)self;

    return createVector3D(1 * noise(perlin_texture->noise, p), 1 * noise(perlin_texture->noise, p),
                          1 * noise(perlin_texture->noise, p));
}

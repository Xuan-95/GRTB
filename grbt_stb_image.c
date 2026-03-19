#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "grbt_stb_image.h"
#include "common.h"
#include "external/stb_image.h"

GRTBImage *createImage(const char *filename) {
    GRTBImage *image          = ALLOCATE(GRTBImage, 1);
    image->bytes_per_pixel    = 3;
    image->fdata              = NULL;
    image->bdata              = NULL;
    image->image_width        = 0;
    image->image_height       = 0;
    image->bytes_per_scanline = 0;

    if (loadImage(image, filename)) {
        return image;
    }
    fprintf(stderr, "ERROR: could not load image file '%s'.\n", filename);
    return NULL;
}

int imageWidth(GRTBImage *image) { return (image->fdata == NULL) ? 0 : image->image_width; }
int imageHeight(GRTBImage *image) { return (image->fdata == NULL) ? 0 : image->image_height; }

int loadImage(GRTBImage *image, const char *filename) {
    int n        = image->bytes_per_pixel;
    image->fdata = stbi_loadf(filename, &image->image_width, &image->image_height, &n, image->bytes_per_pixel);
    if (image->fdata == NULL) {
        return 0;
    }

    image->bytes_per_scanline = imageWidth(image) * image->bytes_per_pixel;
    convertToBytes(image);

    return 1;
}

void convertToBytes(GRTBImage *image) {
    int total_bytes = imageWidth(image) * image->image_height * image->bytes_per_pixel;

    image->bdata = ALLOCATE(unsigned char, total_bytes);

    unsigned char *bptr = image->bdata;
    float         *fptr = image->fdata;

    for (int i = 0; i < total_bytes; i++, bptr++, fptr++) {
        *bptr = floatToByte(*fptr);
    }
}

const unsigned char *pixelData(GRTBImage *image, int x, int y) {
    static unsigned char magenta[] = {255, 0, 255};
    if (image->bdata == NULL)
        return magenta;

    x = clamp(x, 0, imageWidth(image) - 1);
    y = clamp(y, 0, imageHeight(image) - 1);

    return image->bdata + y * image->bytes_per_scanline + x * image->bytes_per_pixel;
}

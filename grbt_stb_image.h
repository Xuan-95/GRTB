#ifndef GRTB_STB_IMAGE_H
#define GRTB_STB_IMAGE_H

#include "common.h"

typedef struct {
    int            bytes_per_pixel;
    float         *fdata;
    unsigned char *bdata;
    int            image_width;
    int            image_height;
    int            bytes_per_scanline;
} GRTBImage;

GRTBImage           *createImage(const char *filename);
int                  loadImage(GRTBImage *image, const char *filename);
void                 convertToBytes(GRTBImage *image);
const unsigned char *pixelData(GRTBImage *image, int x, int y);
int                  imageWidth(GRTBImage *image);
int                  imageHeight(GRTBImage *image);

#endif

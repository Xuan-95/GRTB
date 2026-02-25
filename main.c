#include <stdio.h>
#include <stdlib.h>

#include "color.h"
#include "vector3d.h"

int main(int argc, char *argv[]) {
    int IMAGE_WIDTH = 256;
    int IMAGE_HEIGHT = 256;

    // Header
    printf("P3\n%d %d\n255\n", IMAGE_WIDTH, IMAGE_HEIGHT);
    for (int i = 0; i < IMAGE_HEIGHT; i++) {
        for (int j = 0; j < IMAGE_WIDTH; j++) {
            fprintf(stderr, "\rScanlines remaining: %d ", IMAGE_HEIGHT - j);
            fflush(stderr);

            Vector3D vec = createVector3D((double)i / (IMAGE_WIDTH - 1),
                                          (double)j / (IMAGE_HEIGHT - 1), 0.0);
            Color color = toColor(vec);
            writeColor(stdout, color);
        }
    }
    fprintf(stderr, "\rDone.                 \n");

    return EXIT_SUCCESS;
}

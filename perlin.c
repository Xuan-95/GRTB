#include "perlin.h"
#include "common.h"
#include "memory.h"
#include <math.h>

Perlin *createPerlin(void) {
    Perlin *perlin = ALLOCATE(Perlin, 1);
    for (int i = 0; i < PERLIN_POINT_COUNT; i++) {
        perlin->randfloat[i] = randomDouble(0, 1);
    }

    perlinGeneratePerm(perlin->perm_x);
    perlinGeneratePerm(perlin->perm_y);
    perlinGeneratePerm(perlin->perm_z);
    return perlin;
}

double trilinearInterp(double c[2][2][2], double u, double v, double w) {
    double accum = 0.0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                accum += (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v)) * (k * w + (1 - k) * (1 - w)) *
                         c[i][j][k];
            }
        }
    }
    return accum;
}

double noise(Perlin *perlin, Point3D p) {

    // Split integer and fractional part
    double u = p.x - floor(p.x);
    double v = p.y - floor(p.y);
    double w = p.z - floor(p.z);
    int    i = (int)floor(p.x);
    int    j = (int)floor(p.y);
    int    k = (int)floor(p.z);

    // Hermitian smoothing
    u = u * u * (3 - 2 * u);
    v = v * v * (3 - 2 * v);
    w = w * w * (3 - 2 * w);

    // Sample the corners
    double c[2][2][2];
    for (int di = 0; di < 2; di++) {
        for (int dj = 0; dj < 2; dj++) {
            for (int dk = 0; dk < 2; dk++) {
                c[di][dj][dk] = perlin->randfloat[perlin->perm_x[(i + di) & 255] ^ perlin->perm_y[(j + dj) & 255] ^
                                                  perlin->perm_z[(k + dk) & 255]];
            }
        }
    }
    return trilinearInterp(c, u, v, w);
}

void perlinGeneratePerm(int *p) {
    for (int i = 0; i < PERLIN_POINT_COUNT; i++) {
        p[i] = i;
    }
    permute(p, PERLIN_POINT_COUNT);
}

void permute(int *p, int n) {
    for (int i = n - 1; i > 0; i--) {
        int target = randomInt(0, i);
        int temp   = p[i];
        p[i]       = p[target];
        p[target]  = temp;
    }
}

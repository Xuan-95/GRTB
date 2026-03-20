#include "perlin.h"
#include "common.h"
#include "memory.h"

Perlin *createPerlin(void) {
    Perlin *perlin = ALLOCATE(Perlin, 1);
    for (int i = 0; i < PERLINE_POINT_COUNT; i++) {
        perlin->randfloat[i] = randomDouble(0, 1);
    }

    perlinGeneratePerm(perlin->perm_x);
    perlinGeneratePerm(perlin->perm_y);
    perlinGeneratePerm(perlin->perm_z);
    return perlin;
}

double noise(Perlin *perlin, Point3D p) {
    int i = (int)(4 * p.x) & 255;
    int j = (int)(4 * p.y) & 255;
    int k = (int)(4 * p.z) & 255;

    int idx = perlin->perm_x[i] ^ perlin->perm_y[j] ^ perlin->perm_z[k];
    return perlin->randfloat[idx];
}

void perlinGeneratePerm(int *p) {
    for (int i = 0; i < PERLINE_POINT_COUNT; i++) {
        p[i] = i;
    }
    permute(p, PERLINE_POINT_COUNT);
}

void permute(int *p, int n) {
    for (int i = n - 1; i > 0; i--) {
        int target = randomInt(0, i);
        int temp   = p[i];
        p[i]       = p[target];
        p[target]  = temp;
    }
}

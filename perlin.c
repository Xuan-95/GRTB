#include "perlin.h"
#include "common.h"
#include "memory.h"
#include "vector3d.h"
#include <math.h>

Perlin *createPerlin(void) {
    Perlin *perlin = ALLOCATE(Perlin, 1);
    for (int i = 0; i < PERLIN_POINT_COUNT; i++) {
        perlin->randvec[i] = randomVec3D(-1.0, 1.0);
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

double perlinInterp(Vector3D c[2][2][2], double u, double v, double w) {
    double uu    = u * u * (3 - 2 * u);
    double vv    = v * v * (3 - 2 * v);
    double ww    = w * w * (3 - 2 * w);
    double accum = 0.0;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++) {
                Vector3D weight_v = createVector3D(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) *
                         dot3D(c[i][j][k], weight_v);
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

    // Sample the corners
    Vector3D c[2][2][2];
    for (int di = 0; di < 2; di++) {
        for (int dj = 0; dj < 2; dj++) {
            for (int dk = 0; dk < 2; dk++) {
                c[di][dj][dk] = perlin->randvec[perlin->perm_x[(i + di) & 255] ^ perlin->perm_y[(j + dj) & 255] ^
                                                perlin->perm_z[(k + dk) & 255]];
            }
        }
    }
    return perlinInterp(c, u, v, w);
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

double turbulence(Perlin *perlin, Point3D p, int depth) {
    double  accum  = 0.0;
    double  weight = 1.0;
    Point3D temp_p = p;

    // Add octaves to Perlin noise
    for (int i = 0; i < depth; i++) {
        accum  += weight * noise(perlin, temp_p);
        weight *= 0.5;
        temp_p  = scalarMultiply3D(2, temp_p);
    }
    return fabs(accum);
}

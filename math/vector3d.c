#include "vector3d.h"
#include "../core/common.h"
#include "interval.h"
#include <math.h>

static inline double linearToGamma(double linear_component) {
    if (linear_component > 0) {
        return sqrt(linear_component);
    }
    return 0.0;
}

void writeColor(FILE *file, Color color) {
    Interval intensity = createInterval(0.0, 0.999);

    double   r = linearToGamma(color.cr);
    double   g = linearToGamma(color.cg);
    double   b = linearToGamma(color.cb);

    int      rbyte = (int)(255.999 * clampInterval(&intensity, r));
    int      gbyte = (int)(255.999 * clampInterval(&intensity, g));
    int      bbyte = (int)(255.999 * clampInterval(&intensity, b));

    fprintf(file, "%d %d %d\n", rbyte, gbyte, bbyte);
}

Vector3D randomVec3D(double min, double max) {
    return (Vector3D){.x = randomDouble(min, max), .y = randomDouble(min, max), .z = randomDouble(min, max)};
}

Vector3D randomUnitVec3D(void) {
    // Rejection sampling method
    // NOTE: This is good for CPU, is not for GPU because is a branching method
    while (1) {
        Vector3D p              = randomVec3D(-1.0, 1.0);
        double   length_squared = lengthSquared3D(p);
        if (1e-160 < length_squared && length_squared <= 1) {
            return scalarDivide3D(p, sqrt(length_squared));
        }
    }
}

Vector3D randomOnHemisphere(Vector3D *normal) {
    Vector3D on_unit_sphere = randomUnitVec3D();
    if (dot3D(on_unit_sphere, *normal) > 0) {
        return on_unit_sphere;
    } else {
        return scalarMultiply3D(-1, on_unit_sphere);
    }
}

Vector3D random_in_unit_disk(void) {
    // Rejection sampling
    // NOTE: This is good for CPU, is not for GPU because is a branching method
    while (1) {
        Vector3D p = createVector3D(randomDouble(-1.0, 1.0), randomDouble(-1.0, 1), 0.0);
        if (lengthSquared3D(p) < 1) {
            return p;
        }
    }
}

Vector3D reflectVec3D(Vector3D v, Vector3D n) {
    return diff3D(v, scalarMultiply3D(2, scalarMultiply3D(dot3D(v, n), n)));
}

int nearZero3D(Vector3D v) {
    double s = 1e-8;
    return (fabs(v.x) < s) && (fabs(v.y) < s) && (fabs(v.z) < s);
}

Vector3D refractVec3D(Vector3D uv, Vector3D n, double etai_over_etat) {
    double cos_theta        = dot3D(scalarMultiply3D(-1.0, uv), n);
    cos_theta               = fmin(cos_theta, 1.0);
    Vector3D r_out_perp     = scalarMultiply3D(etai_over_etat, sum3D(uv, scalarMultiply3D(cos_theta, n)));
    Vector3D r_out_parallel = scalarMultiply3D(-sqrt(fabs(1.0 - lengthSquared3D(r_out_perp))), n);
    return sum3D(r_out_perp, r_out_parallel);
}

Vector3D randomCosineDirection(void) {
    double r1 = randomDouble(0, 1);
    double r2 = randomDouble(0, 1);

    double phi = 2 * PI * r1;
    double x   = cos(phi) * sqrt(r2);
    double y   = sin(phi) * sqrt(r2);
    double z   = sqrt(1 - r2);

    return createVector3D(x, y, z);
}

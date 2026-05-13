#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>

#define RGB(r, g, b) ((Color){.cr = (r), .cg = (g), .cb = (b)})

typedef struct {
    union {
        struct {
            double x, y, z;
        };
        struct {
            double cr, cg, cb;
        };
        double e[3];
    };
} Vector3D;

typedef Vector3D       Point3D;
typedef Vector3D       Color;

void                   writeColor(FILE *f, Color color);
Vector3D               randomVec3D(double min, double max);
Vector3D               randomUnitVec3D(void);
Vector3D               randomOnHemisphere(Vector3D *normal);
int                    nearZero3D(Vector3D v);
Vector3D               reflectVec3D(Vector3D v, Vector3D n);
Vector3D               refractVec3D(Vector3D uv, Vector3D n, double etai_over_etat);
Vector3D               random_in_unit_disk(void);
Vector3D               randomCosineDirection(void);

static inline Vector3D createVector3D(double x, double y, double z) { return (Vector3D){.x = x, .y = y, .z = z}; }

static inline Vector3D negate3D(Vector3D v) { return (Vector3D){.x = -v.x, .y = -v.y, .z = -v.z}; }

static inline double   lengthSquared3D(Vector3D v) { return (v.x * v.x) + (v.y * v.y) + (v.z * v.z); }

static inline double   length3D(Vector3D v) { return sqrt(lengthSquared3D(v)); }

static inline void     print3D(FILE *out, Vector3D v) { fprintf(out, "%f %f %f", v.x, v.y, v.z); }

static inline Vector3D diff3D(Vector3D u, Vector3D v) {
    return (Vector3D){.x = u.x - v.x, .y = u.y - v.y, .z = u.z - v.z};
}

static inline double   dot3D(Vector3D u, Vector3D v) { return (u.x * v.x) + (u.y * v.y) + (u.z * v.z); }

static inline Vector3D crossProduct3D(Vector3D u, Vector3D v) {
    return (Vector3D){.x = (u.y * v.z) - (u.z * v.y), .y = (u.z * v.x) - (u.x * v.z), .z = (u.x * v.y) - (u.y * v.x)};
}

static inline Vector3D scalarMultiply3D(double t, Vector3D v) {
    return (Vector3D){.x = t * v.x, .y = t * v.y, .z = t * v.z};
}

static inline Vector3D scalarDivide3D(Vector3D v, double t) { return scalarMultiply3D(1.0 / t, v); }
static inline Vector3D sum3D(Vector3D u, Vector3D v) {
    return (Vector3D){.x = u.x + v.x, .y = u.y + v.y, .z = u.z + v.z};
}

static inline Vector3D unitVector3D(Vector3D v) { return scalarDivide3D(v, length3D(v)); }

static inline Vector3D mul3D(Vector3D u, Vector3D v) {
    return (Vector3D){.x = u.x * v.x, .y = u.y * v.y, .z = u.z * v.z};
}

#endif

#include "ray.h"
#include "camera.h"
#include "vector3d.h"

Ray createRay(Point3D origin, Vector3D direction) {
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    return ray;
}

Point3D rayAt(Ray ray, double t) {
    return sum3D(ray.origin, scalarMultiply3D(t, ray.direction));
}



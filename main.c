#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "ray.h"
#include "vector3d.h"

// Evaluate if a ray hit a sphere
double hitSphere(Point3D *center, double radius, Ray *r) {
    Vector3D oc = diff3D(*center, r->origin);
    double a = dot3D(r->direction, r->direction);
    double h = -dot3D(r->direction, oc);
    double c = dot3D(oc, oc) - pow(radius, 2);
    double discriminant = pow(h, 2) - (a * c);

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-h - sqrt(discriminant)) / a;
    }
}

Color rayColor(Ray *r) {
    Vector3D sphere_center = createVector3D(0.0, 0.0, -1);

    double t = hitSphere(&sphere_center, 0.5, r);
    if (t > 0.0) {
        Vector3D n = unitVector3D(diff3D(rayAt(*r, t), sphere_center));
        return scalarMultiply3D(0.5, createVector3D(n.x + 1, n.y + 1, n.z + 1));
    }

    Vector3D unit_direction = r->direction;
    double a = 0.5 * (unit_direction.y + 1.0);
    return sum3D(scalarMultiply3D((1.0 - a), RGB(1.0, 1.0, 1.0)),
                 scalarMultiply3D(a, RGB(0.5, 0.7, 1.0)));
}

int main(int argc, char *argv[]) {
    // Image
    float ASPECT_RATIO = 16.0 / 9.0;
    int IMAGE_WIDTH = 400;

    int IMAGE_HEIGHT = (int)(IMAGE_WIDTH / ASPECT_RATIO);
    IMAGE_HEIGHT = (IMAGE_HEIGHT < 1) ? 1 : IMAGE_HEIGHT;

    // Camera
    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width =
        viewport_height * ((double)IMAGE_WIDTH / IMAGE_HEIGHT);
    Point3D camera_center = createVector3D(0.0, 0.0, 0.0);

    Vector3D viewport_u = createVector3D(viewport_width, 0.0, 0.0);
    Vector3D viewport_v = createVector3D(0.0, -viewport_height, 0.0);
    Vector3D pixel_delta_u = scalarDivide3D(viewport_u, IMAGE_WIDTH);
    Vector3D pixel_delta_v = scalarDivide3D(viewport_v, IMAGE_HEIGHT);

    Vector3D viewport_upper_left =
        diff3D(camera_center, (Vector3D){0.0, 0.0, focal_length});
    viewport_upper_left =
        diff3D(viewport_upper_left, scalarDivide3D(viewport_u, 2.0));
    viewport_upper_left =
        diff3D(viewport_upper_left, scalarDivide3D(viewport_v, 2.0));

    Point3D pixel00_loc =
        sum3D(viewport_upper_left,
              scalarMultiply3D(0.5, sum3D(pixel_delta_v, pixel_delta_u)));

    // Render
    printf("P3\n%d %d\n255\n", IMAGE_WIDTH, IMAGE_HEIGHT);
    for (int i = 0; i < IMAGE_HEIGHT; i++) {
        fprintf(stderr, "\rScanlines remaining: %d ", IMAGE_HEIGHT - i);
        fflush(stderr);
        for (int j = 0; j < IMAGE_WIDTH; j++) {

            Point3D pixel_center =
                sum3D(sum3D(pixel00_loc, scalarMultiply3D(j, pixel_delta_u)),
                      scalarMultiply3D(i, pixel_delta_v));
            Vector3D ray_direction = diff3D(pixel_center, camera_center);
            Ray r = createRay(camera_center, ray_direction);
            Color color = rayColor(&r);

            writeColor(stdout, color);
        }
    }
    fprintf(stderr, "\rDone.                 \n");

    return EXIT_SUCCESS;
}

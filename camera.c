#include "camera.h"
#include "common.h"
#include "vector3d.h"

static inline Vector3D sampleSquare(void) {
    return createVector3D(randomDouble(-0.5, 0.5), randomDouble(-0.5, 0.5),
                          0.0);
}

Ray getRay(Camera *camera, int i, int j) {
    Vector3D offset = sampleSquare();
    Point3D pixel_sample =
        sum3D(sum3D(camera->pixel00_loc,
                    scalarMultiply3D(j + offset.x, camera->pixel_delta_u)),
              scalarMultiply3D(i + offset.y, camera->pixel_delta_v));
    Vector3D ray_direction = diff3D(pixel_sample, camera->camera_center);
    return createRay(camera->camera_center, ray_direction);
}

void initCamera(Camera *camera) {
    camera->aspect_ratio = 16.0 / 9.0;
    camera->image_width = 400;
    camera->samples_per_pixels = 100;
    camera->pixel_samples_scale = 1.0 / camera->samples_per_pixels;

    camera->image_height = (int)(camera->image_width / camera->aspect_ratio);
    camera->image_height =
        (camera->image_height < 1) ? 1 : camera->image_height;

    camera->focal_length = 1.0;
    camera->viewport_height = 2.0;
    camera->viewport_width =
        camera->viewport_height *
        ((double)camera->image_width / camera->image_height);
    camera->camera_center = createVector3D(0.0, 0.0, 0.0);

    camera->viewport_u = createVector3D(camera->viewport_width, 0.0, 0.0);
    camera->viewport_v = createVector3D(0.0, -camera->viewport_height, 0.0);
    camera->pixel_delta_u =
        scalarDivide3D(camera->viewport_u, camera->image_width);
    camera->pixel_delta_v =
        scalarDivide3D(camera->viewport_v, camera->image_height);

    camera->viewport_upper_left = diff3D(
        camera->camera_center, (Vector3D){0.0, 0.0, camera->focal_length});
    camera->viewport_upper_left = diff3D(
        camera->viewport_upper_left, scalarDivide3D(camera->viewport_u, 2.0));
    camera->viewport_upper_left = diff3D(
        camera->viewport_upper_left, scalarDivide3D(camera->viewport_v, 2.0));

    camera->pixel00_loc =
        sum3D(camera->viewport_upper_left,
              scalarMultiply3D(
                  0.5, sum3D(camera->pixel_delta_v, camera->pixel_delta_u)));
}

Color rayColor(Ray *r, Hittable *world) {
    HitRecord hit_rec;
    if (world->hit(world, r, createInterval(0.0, INFINITY), &hit_rec)) {
        return scalarMultiply3D(0.5, createVector3D(hit_rec.normal.x + 1,
                                                    hit_rec.normal.y + 1,
                                                    hit_rec.normal.z + 1));
    }

    Vector3D unit_direction = unitVector3D(r->direction);
    double a = 0.5 * (unit_direction.y + 1.0);
    return sum3D(scalarMultiply3D((1.0 - a), RGB(1.0, 1.0, 1.0)),
                 scalarMultiply3D(a, RGB(0.5, 0.7, 1.0)));
}

void render(Camera *camera, Hittable *world) {
    printf("P3\n%d %d\n255\n", camera->image_width, camera->image_height);
    for (int i = 0; i < camera->image_height; i++) {
        fprintf(stderr, "\rScanlines remaining: %d ", camera->image_height - i);
        fflush(stderr);
        for (int j = 0; j < camera->image_width; j++) {
            Vector3D pixel_color = createVector3D(0.0, 0.0, 0.0);
            for (int sample = 0; sample < camera->samples_per_pixels;
                 sample++) {
                Ray r = getRay(camera, i, j);
                pixel_color = sum3D(pixel_color, rayColor(&r, world));
            }
            writeColor(stdout, scalarMultiply3D(camera->pixel_samples_scale,
                                                pixel_color));
        }
    }
    fprintf(stderr, "\rDone.                 \n");
}

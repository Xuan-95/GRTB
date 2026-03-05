#include "camera.h"
#include "common.h"
#include "material.h"
#include "vector3d.h"
#include <math.h>

static inline Vector3D sampleSquare(void) {
    return createVector3D(randomDouble(-0.5, 0.5), randomDouble(-0.5, 0.5), 0.0);
}

Ray getRay(Camera *camera, int i, int j) {
    Vector3D offset = sampleSquare();
    Point3D pixel_sample = sum3D(sum3D(camera->pixel00_loc, scalarMultiply3D(j + offset.x, camera->pixel_delta_u)),
                                 scalarMultiply3D(i + offset.y, camera->pixel_delta_v));
    Vector3D ray_origin = (camera->defocus_angle <= 0) ? camera->camera_center : defocus_disk_sample(camera);
    Vector3D ray_direction = diff3D(pixel_sample, ray_origin);
    return createRay(ray_origin, ray_direction);
}

void initCamera(Camera *camera) {
    camera->aspect_ratio = 16.0 / 9.0;
    camera->image_width = 400;
    camera->samples_per_pixels = 100;
    camera->pixel_samples_scale = 1.0 / camera->samples_per_pixels;
    camera->max_depth = 10;

    camera->vfov = 20;
    camera->lookfrom = createVector3D(-2.0, 2.0, 1.0);
    camera->lookat = createVector3D(0.0, 0.0, -1);
    camera->vup = createVector3D(0.0, 1.0, 0.0);
    camera->defocus_angle = 10.0;
    camera->focus_distance = 3.4;

    camera->camera_center = camera->lookfrom;

    camera->image_height = (int)(camera->image_width / camera->aspect_ratio);
    camera->image_height = (camera->image_height < 1) ? 1 : camera->image_height;

    double theta = degrees_to_radians(camera->vfov);
    double h = tan(theta / 2.0);
    camera->viewport_height = 2 * h * camera->focus_distance;
    camera->viewport_width = camera->viewport_height * ((double)camera->image_width / camera->image_height);

    camera->w = unitVector3D(diff3D(camera->lookfrom, camera->lookat));
    camera->u = unitVector3D(crossProduct3D(camera->vup, camera->w));
    camera->v = crossProduct3D(camera->w, camera->u);

    camera->viewport_u = scalarMultiply3D(camera->viewport_width, camera->u);
    camera->viewport_v = scalarMultiply3D(-camera->viewport_height, camera->v);
    camera->pixel_delta_u = scalarDivide3D(camera->viewport_u, camera->image_width);
    camera->pixel_delta_v = scalarDivide3D(camera->viewport_v, camera->image_height);

    camera->viewport_upper_left = diff3D(camera->camera_center, scalarMultiply3D(camera->focus_distance, camera->w));
    camera->viewport_upper_left = diff3D(camera->viewport_upper_left, scalarDivide3D(camera->viewport_u, 2.0));
    camera->viewport_upper_left = diff3D(camera->viewport_upper_left, scalarDivide3D(camera->viewport_v, 2.0));

    camera->pixel00_loc =
        sum3D(camera->viewport_upper_left, scalarMultiply3D(0.5, sum3D(camera->pixel_delta_v, camera->pixel_delta_u)));

    double defocus_radius = camera->focus_distance * tan(degrees_to_radians(camera->defocus_angle / 2.0));
    camera->defocus_disk_u = scalarMultiply3D(defocus_radius, camera->u);
    camera->defocus_disk_v = scalarMultiply3D(defocus_radius, camera->v);
}

Color rayColor(Ray *r, Hittable *world, int depth) {
    if (depth <= 0) {
        return (Color){0.0, 0.0, 0.0};
    }

    HitRecord hit_rec;
    if (world->hit(world, r, createInterval(0.001, INFINITY), &hit_rec)) {
        Ray scattered;
        Color attenuation;
        if (hit_rec.mat->scatter(hit_rec.mat, r, &hit_rec, &attenuation, &scattered)) {
            return mul3D(attenuation, rayColor(&scattered, world, depth - 1));
        } else {
            return (Color){0.0, 0.0, 0.0};
        }
    }

    Vector3D unit_direction = unitVector3D(r->direction);
    double a = 0.5 * (unit_direction.y + 1.0);
    return sum3D(scalarMultiply3D((1.0 - a), RGB(1.0, 1.0, 1.0)), scalarMultiply3D(a, RGB(0.5, 0.7, 1.0)));
}

void render(Camera *camera, Hittable *world) {
    printf("P3\n%d %d\n255\n", camera->image_width, camera->image_height);
    for (int i = 0; i < camera->image_height; i++) {
        fprintf(stderr, "\rScanlines remaining: %d ", camera->image_height - i);
        fflush(stderr);
        for (int j = 0; j < camera->image_width; j++) {
            Color pixel_color = createVector3D(0.0, 0.0, 0.0);
            for (int sample = 0; sample < camera->samples_per_pixels; sample++) {
                Ray r = getRay(camera, i, j);
                pixel_color = sum3D(pixel_color, rayColor(&r, world, camera->max_depth));
            }
            writeColor(stdout, scalarMultiply3D(camera->pixel_samples_scale, pixel_color));
        }
    }
    fprintf(stderr, "\rDone.                 \n");
}

Point3D defocus_disk_sample(Camera *camera) {
    // Returns a random point in the camera defocus disk.
    Vector3D p = random_in_unit_disk();
    Vector3D sample = sum3D(camera->camera_center, scalarMultiply3D(p.x, camera->defocus_disk_u));
    sample = sum3D(sample, scalarMultiply3D(p.y, camera->defocus_disk_v));
    return sample;
}

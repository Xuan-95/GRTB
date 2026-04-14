#include "camera.h"
#include "common.h"
#include "material.h"
#include "pdf.h"
#include "vector3d.h"
#include <math.h>
#include <stdatomic.h>

static inline Vector3D sampleSquare(void) {
    return createVector3D(randomDouble(-0.5, 0.5), randomDouble(-0.5, 0.5), 0.0);
}

Ray getRay(Camera *camera, int i, int j) {
    Vector3D offset        = sampleSquare();
    Point3D  pixel_sample  = sum3D(sum3D(camera->pixel00_loc, scalarMultiply3D(j + offset.x, camera->pixel_delta_u)),
                                   scalarMultiply3D(i + offset.y, camera->pixel_delta_v));
    Vector3D ray_origin    = (camera->defocus_angle <= 0) ? camera->camera_center : defocus_disk_sample(camera);
    Vector3D ray_direction = diff3D(pixel_sample, ray_origin);
    double   ray_time      = randomDouble(0, 1.0);
    return createRay(ray_origin, ray_direction, ray_time);
}

void initCamera(Camera *camera) {

    camera->camera_center = camera->lookfrom;
    camera->image_height  = (int)(camera->image_width / camera->aspect_ratio);
    camera->image_height  = (camera->image_height < 1) ? 1 : camera->image_height;

    double theta            = degrees_to_radians(camera->vfov);
    double h                = tan(theta / 2.0);
    camera->viewport_height = 2 * h * camera->focus_distance;
    camera->viewport_width  = camera->viewport_height * ((double)camera->image_width / camera->image_height);

    camera->w = unitVector3D(diff3D(camera->lookfrom, camera->lookat));
    camera->u = unitVector3D(crossProduct3D(camera->vup, camera->w));
    camera->v = crossProduct3D(camera->w, camera->u);

    camera->viewport_u    = scalarMultiply3D(camera->viewport_width, camera->u);
    camera->viewport_v    = scalarMultiply3D(-camera->viewport_height, camera->v);
    camera->pixel_delta_u = scalarDivide3D(camera->viewport_u, camera->image_width);
    camera->pixel_delta_v = scalarDivide3D(camera->viewport_v, camera->image_height);

    camera->viewport_upper_left = diff3D(camera->camera_center, scalarMultiply3D(camera->focus_distance, camera->w));
    camera->viewport_upper_left = diff3D(camera->viewport_upper_left, scalarDivide3D(camera->viewport_u, 2.0));
    camera->viewport_upper_left = diff3D(camera->viewport_upper_left, scalarDivide3D(camera->viewport_v, 2.0));

    camera->pixel00_loc =
        sum3D(camera->viewport_upper_left, scalarMultiply3D(0.5, sum3D(camera->pixel_delta_v, camera->pixel_delta_u)));

    double defocus_radius  = camera->focus_distance * tan(degrees_to_radians(camera->defocus_angle / 2.0));
    camera->defocus_disk_u = scalarMultiply3D(defocus_radius, camera->u);
    camera->defocus_disk_v = scalarMultiply3D(defocus_radius, camera->v);
}

Color rayColor(Camera *camera, Ray *r, Hittable *world, int depth) {
    if (depth <= 0) {
        return (Color){{{0.0, 0.0, 0.0}}};
    }

    HitRecord hit_rec;
    if (!world->hit(world, r, createInterval(0.001, INFINITY), &hit_rec)) {
        return camera->background;
    }

    Ray    scattered;
    Color  attenuation;
    Color  color_from_emission;
    double scattering_pdf = 0.0;
    double pdf_value      = 1.0;
    if (hit_rec.mat->emitted != NULL) {
        color_from_emission = hit_rec.mat->emitted(hit_rec.mat, &hit_rec, hit_rec.u, hit_rec.v, hit_rec.p);
    } else {
        color_from_emission = RGB(0.0, 0.0, 0.0);
    }

    if (hit_rec.mat->scatter == NULL ||
        !hit_rec.mat->scatter(hit_rec.mat, r, &hit_rec, &attenuation, &scattered, &pdf_value)) {
        return color_from_emission;
    }

    CosinePdf s_pdf;
    initCosinePdf(&s_pdf, hit_rec.normal);
    Pdf *surface_pdf = (Pdf *)&s_pdf;
    scattered        = createRay(hit_rec.p, surface_pdf->generate(surface_pdf), r->time);
    pdf_value        = surface_pdf->value(surface_pdf, scattered.direction);

    if (hit_rec.mat->scatteringPdf != NULL) {
        scattering_pdf = hit_rec.mat->scatteringPdf(hit_rec.mat, r, &hit_rec, &scattered);
        pdf_value      = scattering_pdf;
    }
    Color color_from_scatter =
        mul3D(scalarMultiply3D(scattering_pdf, attenuation), rayColor(camera, &scattered, world, depth - 1));
    color_from_scatter = scalarDivide3D(color_from_scatter, pdf_value);

    return sum3D(color_from_emission, color_from_scatter);
}

void render(Camera *camera, Hittable *world) {
    initCamera(camera);
    printf("P3\n%d %d\n255\n", camera->image_width, camera->image_height);

    Color     *pixels = malloc(camera->image_width * camera->image_height * sizeof(Color));

    atomic_int completed = 0;
#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < camera->image_height; i++) {
        for (int j = 0; j < camera->image_width; j++) {
            Color pixel_color = createVector3D(0.0, 0.0, 0.0);
            for (int sample = 0; sample < camera->samples_per_pixel; sample++) {
                Ray r       = getRay(camera, i, j);
                pixel_color = sum3D(pixel_color, rayColor(camera, &r, world, camera->max_depth));
            }
            pixels[i * camera->image_width + j] = scalarMultiply3D(camera->pixel_samples_scale, pixel_color);
        }
        int done = atomic_fetch_add(&completed, 1) + 1;
        fprintf(stderr, "\rScanlines completed: %d/%d", done, camera->image_height);
        fflush(stderr);
    }
    for (int i = 0; i < camera->image_height; i++) {
        for (int j = 0; j < camera->image_width; j++) {
            writeColor(stdout, pixels[i * camera->image_width + j]);
        }
    }
    free(pixels);

    fprintf(stderr, "\rDone.                 \n");
}

Point3D defocus_disk_sample(Camera *camera) {
    // Returns a random point in the camera defocus disk.
    Vector3D p      = random_in_unit_disk();
    Vector3D sample = sum3D(camera->camera_center, scalarMultiply3D(p.x, camera->defocus_disk_u));
    sample          = sum3D(sample, scalarMultiply3D(p.y, camera->defocus_disk_v));
    return sample;
}

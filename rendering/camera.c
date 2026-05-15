#include "camera.h"
#include "../core/common.h"
#include "../materials/material.h"
#include "../math/vector3d.h"
#include "pdf.h"
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

Color rayColor(Camera *camera, Ray *initial_ray, Hittable *world, int max_depth, Hittable *lights) {
    Color accumulated_color   = RGB(0.0, 0.0, 0.0);
    Color current_attenuation = RGB(1.0, 1.0, 1.0);
    Ray   current_ray         = *initial_ray;

    for (int i = 0; i < max_depth; i++) {
        // Check if there is an hit
        HitRecord     hit_rec;
        ScatterRecord scatter_rec;
        if (!hittableHit(world, &current_ray, createInterval(0.001, INFINITY), &hit_rec)) {
            return sum3D(accumulated_color, mul3D(current_attenuation, camera->background));
        }

        // Manage emission
        Color color_from_emission = RGB(0.0, 0.0, 0.0);
        color_from_emission       = materialEmitted(hit_rec.mat, &hit_rec, hit_rec.u, hit_rec.v, hit_rec.p);

        accumulated_color = sum3D(accumulated_color, mul3D(current_attenuation, color_from_emission));

        // Manage scattering
        if (!materialScatter(hit_rec.mat, &current_ray, &hit_rec, &scatter_rec)) {
            break;
        }

        // Reflectance materials do not have PDF
        if (scatter_rec.skip_pdf) {
            current_attenuation = mul3D(current_attenuation, scatter_rec.attenuation);
            current_ray         = scatter_rec.skip_pdf_ray;
            continue;
        }

        // Evaluate next ray with PDF
        Pdf *pdf;
        Pdf  hittable_pdf;
        Pdf  mixture_pdf;

        if (lights != NULL) {
            initHittablePdf(&hittable_pdf, lights, hit_rec.p);
            initMixturePdf(&mixture_pdf, &hittable_pdf, scatter_rec.pdf);
            pdf = &mixture_pdf;
        } else {
            pdf = scatter_rec.pdf;
        }

        Vector3D new_direction  = pdfGenerate(pdf);
        double   pdf_val        = pdfValue(pdf, new_direction);
        double   scattering_pdf = materialScatteringPdf(hit_rec.mat, &current_ray, &hit_rec,
                                                        &(Ray){hit_rec.p, new_direction, current_ray.time});

        // Update attenuation = (scatter_attenuation * scattering_pdf) / pdf_val
        Color weight = scalarMultiply3D(scattering_pdf, scatter_rec.attenuation);
        weight       = scalarDivide3D(weight, pdf_val);

        current_attenuation = mul3D(current_attenuation, weight);

        // Create next ray
        current_ray = createRay(hit_rec.p, new_direction, current_ray.time);

        if (scatter_rec.pdf != NULL) {
            free(scatter_rec.pdf);
        }

        if (current_attenuation.x < 1e-5 && current_attenuation.y < 1e-5 && current_attenuation.z < 1e-5)
            break;
    }

    return accumulated_color;
}

void render(Camera *camera, Hittable *world, Hittable *lights) {
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
                pixel_color = sum3D(pixel_color, rayColor(camera, &r, world, camera->max_depth, lights));
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

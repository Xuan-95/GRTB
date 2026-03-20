#include "common.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bvh.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"
#include "vector3d.h"

void bouncingSpheres(void) {
    HittableList world;
    initHittableList(&world);

    // Ground
    Texture  *checker = createCheckerTextureRGB(0.32, createVector3D(0.2, 0.3, 0.1), createVector3D(0.9, 0.9, 0.9));
    Material *ground_material = createLambertianFromTexture(checker);
    addObject(&world, createSphere(createVector3D(0, -1000, 0), 1000, ground_material));

    // Random spheres
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double  choose_mat = randomDouble(0, 1);
            Point3D center     = createVector3D(a + 0.9 * randomDouble(0, 1), 0.2, b + 0.9 * randomDouble(0, 1));

            if (length3D(diff3D(center, createVector3D(4, 0.2, 0))) > 0.9) {
                Material *sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    Color albedo     = mul3D(randomVec3D(0, 1), randomVec3D(0, 1));
                    sphere_material  = createLambertian(albedo);
                    Point3D center_2 = sum3D(center, createVector3D(0.0, randomDouble(0.0, 0.5), 0.0));
                    addObject(&world, createMovingSphere(center, center_2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    Color  albedo   = randomVec3D(0.5, 1);
                    double fuzz     = randomDouble(0, 0.5);
                    sphere_material = createMetal(albedo, fuzz);
                    addObject(&world, createSphere(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = createDielectric(1.5);
                    addObject(&world, createSphere(center, 0.2, sphere_material));
                }
            }
        }
    }

    // Three main spheres
    Material *material1 = createDielectric(1.5);
    addObject(&world, createSphere(createVector3D(0, 1, 0), 1.0, material1));

    Material *material2 = createLambertian(createVector3D(0.4, 0.2, 0.1));
    addObject(&world, createSphere(createVector3D(-4, 1, 0), 1.0, material2));

    Material *material3 = createMetal(createVector3D(0.7, 0.6, 0.5), 0.0);
    addObject(&world, createSphere(createVector3D(4, 1, 0), 1.0, material3));

    Hittable *bvh = createBvhFromList(&world);

    Camera    camera;
    camera.aspect_ratio        = 16.0 / 9.0;
    camera.image_width         = 400;
    camera.samples_per_pixel   = 500;
    camera.pixel_samples_scale = 1.0 / camera.samples_per_pixel;
    camera.max_depth           = 50;
    camera.vfov                = 20;
    camera.lookfrom            = createVector3D(13.0, 2.0, 3.0);
    camera.lookat              = createVector3D(0.0, 0.0, 0.0);
    camera.vup                 = createVector3D(0.0, 1.0, 0.0);
    camera.defocus_angle       = 0.6;
    camera.focus_distance      = 10.0;

    render(&camera, bvh);
    return;
}

void checkeredSpheres(void) {
    HittableList world;
    initHittableList(&world);

    Texture *checker =
        createCheckerTexture(0.32, createSolidColorRGB(0.2, 0.3, 0.1), createSolidColorRGB(0.9, 0.9, 0.9));
    Material *material = createLambertianFromTexture(checker);

    addObject(&world, createSphere(createVector3D(0, -10, 0), 10, material));
    addObject(&world, createSphere(createVector3D(0, 10, 0), 10, material));

    Hittable *bvh = createBvhFromList(&world);

    Camera    camera;
    camera.aspect_ratio        = 16.0 / 9.0;
    camera.image_width         = 400;
    camera.samples_per_pixel   = 100;
    camera.pixel_samples_scale = 1.0 / camera.samples_per_pixel;
    camera.max_depth           = 50;
    camera.vfov                = 20;
    camera.lookfrom            = createVector3D(13, 2, 3);
    camera.lookat              = createVector3D(0, 0, 0);
    camera.vup                 = createVector3D(0, 1, 0);
    camera.defocus_angle       = 0;
    camera.focus_distance      = 10.0;

    render(&camera, bvh);
}

void earth(void) {

    Camera camera;
    camera.aspect_ratio        = 16.0 / 9.0;
    camera.image_width         = 400;
    camera.samples_per_pixel   = 100;
    camera.pixel_samples_scale = 1.0 / camera.samples_per_pixel;
    camera.max_depth           = 50;
    camera.vfov                = 20;
    camera.lookfrom            = createVector3D(0, 0, 12);
    camera.lookat              = createVector3D(0, 0, 0);
    camera.vup                 = createVector3D(0, 1, 0);
    camera.defocus_angle       = 0;
    camera.focus_distance      = 10.0;

    HittableList world;
    initHittableList(&world);

    Texture  *earth_texture = createImageTexture("images/earthmap.jpg");
    Material *earth_surface = createLambertianFromTexture(earth_texture);
    Hittable *globe         = createSphere(createVector3D(0.0, 0.0, 0.0), 2, earth_surface);

    addObject(&world, globe);
    render(&camera, (Hittable *)&world);
}

void perlinSpheres(void) {
    HittableList world;
    initHittableList(&world);

    Texture  *perlin_texture = createPerlinTexture(4);
    Material *perlin_surface = createLambertianFromTexture(perlin_texture);
    Hittable *ground         = createSphere(createVector3D(0.0, -1000, 0.0), 1000, perlin_surface);
    Hittable *sphere         = createSphere(createVector3D(0.0, 2.0, 0.0), 2, perlin_surface);

    addObject(&world, ground);
    addObject(&world, sphere);

    Camera camera;

    camera.aspect_ratio        = 16.0 / 9.0;
    camera.image_width         = 400;
    camera.samples_per_pixel   = 100;
    camera.pixel_samples_scale = 1.0 / camera.samples_per_pixel;
    camera.max_depth           = 50;
    camera.vfov                = 20;
    camera.lookfrom            = createVector3D(13, 2, 3);
    camera.lookat              = createVector3D(0, 0, 0);
    camera.vup                 = createVector3D(0, 1, 0);

    camera.defocus_angle  = 0;
    camera.focus_distance = 10;

    render(&camera, (Hittable *)&world);
}

int main(void) {
    switch (4) {
    case 1: {
        bouncingSpheres();
        break;
    }
    case 2: {
        checkeredSpheres();
        break;
    }
    case 3: {
        earth();
        break;
    }
    case 4: {
        perlinSpheres();
        break;
    }
    }
}

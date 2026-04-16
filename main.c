#include "common.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
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
    camera.background          = RGB(0.7, 0.8, 1.0);

    HittableList lights;
    initHittableList(&lights);

    render(&camera, bvh, (Hittable *)&lights);
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
    camera.background          = RGB(0.7, 0.8, 1.0);

    HittableList lights;
    initHittableList(&lights);

    render(&camera, bvh, (Hittable *)&lights);
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
    camera.background          = RGB(0.7, 0.8, 1.0);

    HittableList world;
    initHittableList(&world);

    Texture  *earth_texture = createImageTexture("images/earthmap.jpg");
    Material *earth_surface = createLambertianFromTexture(earth_texture);
    Hittable *globe         = createSphere(createVector3D(0.0, 0.0, 0.0), 2, earth_surface);

    addObject(&world, globe);
    HittableList lights;
    initHittableList(&lights);

    render(&camera, (Hittable *)&world, (Hittable *)&lights);
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
    camera.background     = RGB(0.7, 0.8, 1.0);

    HittableList lights;
    initHittableList(&lights);

    render(&camera, (Hittable *)&world, (Hittable *)&lights);
}

void quads(void) {
    HittableList world;
    initHittableList(&world);

    Material *left_red     = createLambertian(createVector3D(1.0, 0.2, 0.2));
    Material *back_green   = createLambertian(createVector3D(0.2, 1.0, 0.2));
    Material *right_blue   = createLambertian(createVector3D(0.2, 0.2, 1.0));
    Material *upper_orange = createLambertian(createVector3D(1.0, 0.5, 0.0));
    Material *lower_teal   = createLambertian(createVector3D(0.2, 0.8, 0.8));

    addObject(&world,
              createQuad(createVector3D(-3, -2, 5), createVector3D(0, 0, -4), createVector3D(0, 4, 0), left_red));
    addObject(&world,
              createQuad(createVector3D(-2, -2, 0), createVector3D(4, 0, 0), createVector3D(0, 4, 0), back_green));
    addObject(&world,
              createQuad(createVector3D(3, -2, 1), createVector3D(0, 0, 4), createVector3D(0, 4, 0), right_blue));
    addObject(&world,
              createQuad(createVector3D(-2, 3, 1), createVector3D(4, 0, 0), createVector3D(0, 0, 4), upper_orange));
    addObject(&world,
              createQuad(createVector3D(-2, -3, 5), createVector3D(4, 0, 0), createVector3D(0, 0, -4), lower_teal));

    Camera camera;
    initCamera(&camera);
    camera.aspect_ratio        = 1.0;
    camera.image_width         = 400;
    camera.samples_per_pixel   = 100;
    camera.pixel_samples_scale = 1.0 / camera.samples_per_pixel;
    camera.max_depth           = 50;
    camera.vfov                = 80;
    camera.lookfrom            = createVector3D(0, 0, 9);
    camera.lookat              = createVector3D(0, 0, 0);
    camera.vup                 = createVector3D(0, 1, 0);
    camera.defocus_angle       = 0;
    camera.focus_distance      = 10;
    camera.background          = RGB(0.7, 0.8, 1.0);

    Hittable    *bvh = createBvhFromList(&world);
    HittableList lights;
    initHittableList(&lights);

    render(&camera, bvh, (Hittable *)&lights);
}

void simple_light(void) {
    HittableList world;
    initHittableList(&world);

    Texture  *perlin_texture = createPerlinTexture(4);
    Material *perlin_surface = createLambertianFromTexture(perlin_texture);
    Hittable *ground         = createSphere(createVector3D(0.0, -1000, 0.0), 1000, perlin_surface);
    Hittable *sphere         = createSphere(createVector3D(0.0, 2.0, 0.0), 2, perlin_surface);

    addObject(&world, ground);
    addObject(&world, sphere);

    Material *diffuse_light = createDiffuseLightFromColor(RGB(4, 4, 4));
    Hittable *quad =
        createQuad(createVector3D(3, 1, -2), createVector3D(2, 0, 0), createVector3D(0, 2, 0), diffuse_light);
    addObject(&world, quad);

    Camera camera;
    initCamera(&camera);
    camera.aspect_ratio        = 16.0 / 9.0;
    camera.image_width         = 400;
    camera.samples_per_pixel   = 100;
    camera.pixel_samples_scale = 1.0 / camera.samples_per_pixel;
    camera.max_depth           = 50;
    camera.vfov                = 20;
    camera.lookfrom            = createVector3D(26, 3, 6);
    camera.lookat              = createVector3D(0, 2, 0);
    camera.vup                 = createVector3D(0, 1, 0);
    camera.defocus_angle       = 0;
    camera.focus_distance      = 10;
    camera.background          = RGB(0, 0, 0);

    HittableList lights;
    initHittableList(&lights);

    render(&camera, (Hittable *)&world, (Hittable *)&lights);
}

void cornell_box(void) {
    HittableList world;
    initHittableList(&world);

    Material *red   = createLambertian(RGB(0.65, 0.05, 0.05));
    Material *white = createLambertian(RGB(0.73, 0.73, 0.73));
    Material *green = createLambertian(RGB(0.12, 0.45, 0.15));
    Material *light = createDiffuseLightFromColor(RGB(15, 15, 15));

    addObject(&world,
              createQuad(createVector3D(555, 0, 0), createVector3D(0, 555, 0), createVector3D(0, 0, 555), green));
    addObject(&world, createQuad(createVector3D(0, 0, 0), createVector3D(0, 555, 0), createVector3D(0, 0, 555), red));
    addObject(&world,
              createQuad(createVector3D(343, 554, 332), createVector3D(-130, 0, 0), createVector3D(0, 0, -105), light));
    addObject(&world, createQuad(createVector3D(0, 0, 0), createVector3D(555, 0, 0), createVector3D(0, 0, 555), white));
    addObject(&world,
              createQuad(createVector3D(555, 555, 555), createVector3D(-555, 0, 0), createVector3D(0, 0, -555), white));
    addObject(&world,
              createQuad(createVector3D(0, 0, 555), createVector3D(555, 0, 0), createVector3D(0, 555, 0), white));

    Hittable *box_1 = (Hittable *)createBox(createVector3D(0, 0, 0), createVector3D(165, 330, 165), white);
    box_1           = createRotateY(box_1, 15);
    box_1           = createTranslate(box_1, createVector3D(265, 0, 295));
    addObject(&world, box_1);

    Hittable *box_2 = (Hittable *)createBox(createVector3D(0, 0, 0), createVector3D(165, 165, 165), white);
    box_2           = createRotateY(box_2, -18);
    box_2           = createTranslate(box_2, createVector3D(130, 0, 65));
    addObject(&world, box_2);

    HittableList lights;
    initHittableList(&lights);
    addObject(&lights,
              createQuad(createVector3D(343, 554, 332), createVector3D(-130, 0, 0), createVector3D(0, 0, -105), light));

    Camera camera;
    initCamera(&camera);
    camera.aspect_ratio        = 1.0;
    camera.image_width         = 600;
    camera.samples_per_pixel   = 10;
    camera.pixel_samples_scale = 1.0 / camera.samples_per_pixel;
    camera.max_depth           = 50;
    camera.background          = createVector3D(0, 0, 0);
    camera.vfov                = 40;
    camera.lookfrom            = createVector3D(278, 278, -800);
    camera.lookat              = createVector3D(278, 278, 0);
    camera.vup                 = createVector3D(0, 1, 0);
    camera.defocus_angle       = 0;
    camera.focus_distance      = 10;
    camera.background          = RGB(0, 0, 0);

    Hittable *bvh = createBvhFromList(&world);
    render(&camera, bvh, (Hittable *)&lights);
}

void cornell_smoke(void) {
    HittableList world;
    initHittableList(&world);

    Material *red   = createLambertian(RGB(0.65, 0.05, 0.05));
    Material *white = createLambertian(RGB(0.73, 0.73, 0.73));
    Material *green = createLambertian(RGB(0.12, 0.45, 0.15));
    Material *light = createDiffuseLightFromColor(RGB(7, 7, 7));

    addObject(&world,
              createQuad(createVector3D(555, 0, 0), createVector3D(0, 555, 0), createVector3D(0, 0, 555), green));
    addObject(&world, createQuad(createVector3D(0, 0, 0), createVector3D(0, 555, 0), createVector3D(0, 0, 555), red));
    addObject(&world,
              createQuad(createVector3D(113, 554, 127), createVector3D(330, 0, 0), createVector3D(0, 0, 305), light));
    addObject(&world,
              createQuad(createVector3D(0, 555, 0), createVector3D(555, 0, 0), createVector3D(0, 0, 555), white));
    addObject(&world, createQuad(createVector3D(0, 0, 0), createVector3D(555, 0, 0), createVector3D(0, 0, 555), white));
    addObject(&world,
              createQuad(createVector3D(0, 0, 555), createVector3D(555, 0, 0), createVector3D(0, 555, 0), white));

    Hittable *box_1 = (Hittable *)createBox(createVector3D(0, 0, 0), createVector3D(165, 330, 165), white);
    box_1           = createRotateY(box_1, 15);
    box_1           = createTranslate(box_1, createVector3D(265, 0, 295));

    Hittable *box_2 = (Hittable *)createBox(createVector3D(0, 0, 0), createVector3D(165, 165, 165), white);
    box_2           = createRotateY(box_2, -18);
    box_2           = createTranslate(box_2, createVector3D(130, 0, 65));

    addObject(&world, createConstantMediumFromColor(box_1, 0.01, RGB(0, 0, 0)));
    addObject(&world, createConstantMediumFromColor(box_2, 0.01, RGB(1, 1, 1)));

    Camera camera;
    initCamera(&camera);
    camera.aspect_ratio        = 1.0;
    camera.image_width         = 600;
    camera.samples_per_pixel   = 200;
    camera.pixel_samples_scale = 1.0 / camera.samples_per_pixel;
    camera.max_depth           = 50;
    camera.vfov                = 40;
    camera.lookfrom            = createVector3D(278, 278, -800);
    camera.lookat              = createVector3D(278, 278, 0);
    camera.vup                 = createVector3D(0, 1, 0);
    camera.defocus_angle       = 0;
    camera.focus_distance      = 10;
    camera.background          = RGB(0, 0, 0);

    Hittable    *bvh = createBvhFromList(&world);
    HittableList lights;
    initHittableList(&lights);

    render(&camera, bvh, (Hittable *)&lights);
}

void final_scene(int image_width, int samples_per_pixel, int max_depth) {
    // --- Ground boxes ---
    HittableList boxes1;
    initHittableList(&boxes1);
    Material *ground = createLambertian(RGB(0.48, 0.83, 0.53));

    int       boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            double w  = 100.0;
            double x0 = -1000.0 + i * w;
            double z0 = -1000.0 + j * w;
            double y0 = 0.0;
            double x1 = x0 + w;
            double y1 = randomDouble(1, 101);
            double z1 = z0 + w;
            addObject(&boxes1, (Hittable *)createBox(createVector3D(x0, y0, z0), createVector3D(x1, y1, z1), ground));
        }
    }

    HittableList world;
    initHittableList(&world);

    addObject(&world, createBvhFromList(&boxes1));

    // --- Light ---
    Material *light = createDiffuseLightFromColor(RGB(7, 7, 7));
    addObject(&world,
              createQuad(createVector3D(123, 554, 147), createVector3D(300, 0, 0), createVector3D(0, 0, 265), light));

    // --- Moving sphere ---
    Point3D   center1         = createVector3D(400, 400, 200);
    Point3D   center2         = createVector3D(430, 400, 200);
    Material *sphere_material = createLambertian(RGB(0.7, 0.3, 0.1));
    addObject(&world, createMovingSphere(center1, center2, 50, sphere_material));

    // --- Glass + metal spheres ---
    addObject(&world, createSphere(createVector3D(260, 150, 45), 50, createDielectric(1.5)));
    addObject(&world, createSphere(createVector3D(0, 150, 145), 50, createMetal(RGB(0.8, 0.8, 0.9), 1.0)));

    // --- Volumetric spheres ---
    Hittable *boundary = createSphere(createVector3D(360, 150, 145), 70, createDielectric(1.5));
    addObject(&world, boundary);
    addObject(&world, createConstantMediumFromColor(boundary, 0.2, RGB(0.2, 0.4, 0.9)));
    boundary = createSphere(createVector3D(0, 0, 0), 5000, createDielectric(1.5));
    addObject(&world, createConstantMediumFromColor(boundary, 0.0001, RGB(1, 1, 1)));

    // --- Earth sphere ---
    Material *emat = createLambertianFromTexture(createImageTexture("images/earthmap.jpg"));
    addObject(&world, createSphere(createVector3D(400, 200, 400), 100, emat));

    // --- Noise sphere ---
    Texture *pertext = createPerlinTexture(0.2);
    addObject(&world, createSphere(createVector3D(220, 280, 300), 80, createLambertianFromTexture(pertext)));

    // --- White sphere cloud ---
    HittableList boxes2;
    initHittableList(&boxes2);
    Material *white = createLambertian(RGB(0.73, 0.73, 0.73));
    int       ns    = 1000;
    for (int j = 0; j < ns; j++) {
        addObject(&boxes2, createSphere(randomVec3D(0, 165), 10, white));
    }

    Hittable *cloud = createBvhFromList(&boxes2);
    cloud           = createRotateY(cloud, 15);
    cloud           = createTranslate(cloud, createVector3D(-100, 270, 395));
    addObject(&world, cloud);

    // --- Camera ---
    Camera camera;
    initCamera(&camera);
    camera.aspect_ratio        = 1.0;
    camera.image_width         = image_width;
    camera.samples_per_pixel   = samples_per_pixel;
    camera.pixel_samples_scale = 1.0 / camera.samples_per_pixel;
    camera.max_depth           = max_depth;
    camera.vfov                = 40;
    camera.lookfrom            = createVector3D(478, 278, -600);
    camera.lookat              = createVector3D(278, 278, 0);
    camera.vup                 = createVector3D(0, 1, 0);
    camera.defocus_angle       = 0;
    camera.focus_distance      = 10;
    camera.background          = RGB(0, 0, 0);

    Hittable    *bvh = createBvhFromList(&world);
    HittableList lights;
    initHittableList(&lights);

    render(&camera, bvh, (Hittable *)&lights);
}

int main(void) {
    switch (7) {
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
    case 5: {
        quads();
        break;
    }
    case 6: {
        simple_light();
        break;
    }
    case 7: {
        cornell_box();
        break;
    }
    case 8: {
        cornell_smoke();
        break;
    }
    case 9: {
        final_scene(800, 10000, 40);
        break;
    }
    case 10: {
        final_scene(400, 250, 4);
        break;
    }
    }
}

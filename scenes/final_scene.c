#include "../core/common.h"
#include "../hittables/bvh.h"
#include "../hittables/hittable.h"
#include "../hittables/hittable_list.h"
#include "../hittables/quad.h"
#include "../hittables/sphere.h"
#include "../materials/material.h"
#include "../math/vector3d.h"
#include "../rendering/camera.h"
#include "../textures/texture.h"

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

    addObject(&world, createLinearBvhFromList(&boxes1));

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

    Hittable *cloud = createLinearBvhFromList(&boxes2);
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

    Hittable    *bvh = createLinearBvhFromList(&world);
    HittableList lights;
    initHittableList(&lights);
    addObject(&lights,
              createQuad(createVector3D(123, 554, 147), createVector3D(300, 0, 0), createVector3D(0, 0, 265), light));

    render(&camera, bvh, (Hittable *)&lights);
}

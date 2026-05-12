#include "../core/common.h"
#include "../hittables/bvh.h"
#include "../rendering/camera.h"
#include "../hittables/hittable.h"
#include "../hittables/hittable_list.h"
#include "../materials/material.h"
#include "../hittables/quad.h"
#include "../hittables/sphere.h"
#include "../math/vector3d.h"

void cornell_box(void) {
    HittableList world;
    initHittableList(&world);

    Material *red   = createLambertian(RGB(0.65, 0.05, 0.05));
    Material *white = createLambertian(RGB(0.73, 0.73, 0.73));
    Material *green = createLambertian(RGB(0.12, 0.45, 0.15));
    Material *light = createDiffuseLightFromColor(RGB(15, 15, 15));
    Material *glass = createDielectric(1.5);

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

    Hittable *sphere = createSphere(createVector3D(190, 90, 190), 90, glass);
    addObject(&world, sphere);

    HittableList lights;
    initHittableList(&lights);
    addObject(&lights,
              createQuad(createVector3D(343, 554, 332), createVector3D(-130, 0, 0), createVector3D(0, 0, -105), light));

    Camera camera;
    initCamera(&camera);
    camera.aspect_ratio        = 1.0;
    camera.image_width         = 600;
    camera.samples_per_pixel   = 2000;
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

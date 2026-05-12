#include "../common.h"
#include "../bvh.h"
#include "../camera.h"
#include "../hittable.h"
#include "../hittable_list.h"
#include "../material.h"
#include "../quad.h"
#include "../vector3d.h"

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

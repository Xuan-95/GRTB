#include "../core/common.h"
#include "../hittables/bvh.h"
#include "../hittables/hittable.h"
#include "../hittables/hittable_list.h"
#include "../hittables/quad.h"
#include "../materials/material.h"
#include "../math/vector3d.h"
#include "../rendering/camera.h"

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

    Hittable *bvh = createBvhFromList(&world);

    render(&camera, bvh, NULL);
}

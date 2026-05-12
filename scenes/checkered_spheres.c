#include "../common.h"
#include "../bvh.h"
#include "../camera.h"
#include "../hittable.h"
#include "../hittable_list.h"
#include "../material.h"
#include "../sphere.h"
#include "../texture.h"
#include "../vector3d.h"

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

#include "../core/common.h"
#include "../rendering/camera.h"
#include "../hittables/hittable.h"
#include "../hittables/hittable_list.h"
#include "../materials/material.h"
#include "../hittables/sphere.h"
#include "../textures/texture.h"
#include "../math/vector3d.h"

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

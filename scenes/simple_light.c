#include "../core/common.h"
#include "../hittables/hittable.h"
#include "../hittables/hittable_list.h"
#include "../hittables/quad.h"
#include "../hittables/sphere.h"
#include "../materials/material.h"
#include "../math/vector3d.h"
#include "../rendering/camera.h"
#include "../textures/texture.h"

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
    addObject(&lights,
              createQuad(createVector3D(3, 1, -2), createVector3D(2, 0, 0), createVector3D(0, 2, 0), diffuse_light));

    Scene *scene = createSceneFromList(&world);

    render(&camera, scene, (Hittable *)&lights);
}

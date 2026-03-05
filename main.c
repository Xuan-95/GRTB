#include "common.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "vector3d.h"

int main(int argc, char *argv[]) {
    // Materials
    Material *material_ground = createLambertian(createVector3D(0.8, 0.8, 0.0));
    Material *material_center = createLambertian(createVector3D(0.1, 0.2, 0.5));
    Material *material_left = createDielectric(1.50);
    Material *material_bubble = createDielectric(1.00 / 1.50);
    Material *material_right = createMetal(createVector3D(0.8, 0.6, 0.2), 1.0);

    // World
    HittableList world;
    initHittableList(&world);
    addObject(&world, createSphere(createVector3D(0.0, -100.5, -1.0), 100.0,
                                   material_ground));
    addObject(&world, createSphere(createVector3D(0.0, 0.0, -1.2), 0.5,
                                   material_center));
    addObject(&world, createSphere(createVector3D(-1.0, 0.0, -1.0), 0.5,
                                   material_left));
    addObject(&world, createSphere(createVector3D(-1.0, 0.0, -1.0), 0.4,
                                   material_bubble));
    addObject(&world, createSphere(createVector3D(1.0, 0.0, -1.0), 0.5,
                                   material_right));

    Camera cam;
    initCamera(&cam);
    render(&cam, (Hittable *)&world);
    return EXIT_SUCCESS;
}

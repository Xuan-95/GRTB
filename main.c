#include "common.h"
#include <stdio.h>
#include <stdlib.h>

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vector3d.h"

int main(int argc, char *argv[]) {

    // World
    HittableList world;
    initHittableList(&world);
    addObject(&world, createSphere(createVector3D(0.0, 0.0, -1), 0.5));
    addObject(&world, createSphere(createVector3D(0.0, -100.5, -1), 100));

    Camera cam;
    initCamera(&cam);

    render(&cam, (Hittable *)&world);
    return EXIT_SUCCESS;
}

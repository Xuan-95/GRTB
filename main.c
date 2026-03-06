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
    HittableList world;
    initHittableList(&world);

    // Ground
    Material *ground_material = createLambertian(createVector3D(0.5, 0.5, 0.5));
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
                    Color albedo    = mul3D(randomVec3D(0, 1), randomVec3D(0, 1));
                    sphere_material = createLambertian(albedo);
                } else if (choose_mat < 0.95) {
                    // metal
                    Color  albedo   = randomVec3D(0.5, 1);
                    double fuzz     = randomDouble(0, 0.5);
                    sphere_material = createMetal(albedo, fuzz);
                } else {
                    // glass
                    sphere_material = createDielectric(1.5);
                }
                addObject(&world, createSphere(center, 0.2, sphere_material));
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

    Camera cam;
    initCamera(&cam);

    render(&cam, (Hittable *)&world);
    return EXIT_SUCCESS;
}

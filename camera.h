#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "hittable.h"

typedef struct {
    float aspect_ratio;
    int image_width;
    int image_height;
    float focal_length;
    float viewport_height;
    float viewport_width;
    Point3D camera_center;
    Vector3D viewport_u;
    Vector3D viewport_v;
    Vector3D pixel_delta_u;
    Vector3D pixel_delta_v;
    Vector3D viewport_upper_left;
    Point3D pixel00_loc;
} Camera;

// TODO: camera parameters are hardcoded inside init
void initCamera(Camera *camera);
void render(Camera *camera, Hittable *world);

#endif // !CAMERA_H

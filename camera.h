#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "ray.h"

typedef struct {
    float aspect_ratio;
    int image_width;
    int image_height;
    int samples_per_pixels;
    float pixel_samples_scale;
    int max_depth;
    int vfov;
    Vector3D lookfrom;
    Vector3D lookat;
    Vector3D vup;
    Vector3D v;
    Vector3D u;
    Vector3D w;
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
Ray getRay(Camera *camera, int i, int j);

#endif // !CAMERA_H

# GRTB - Ray Tracer in C

A C implementation of the ray tracer described in Peter Shirley's [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) series, extended with features from _The Next Week_ and _The Rest of Your Life_.

## Features

### Materials
- **Lambertian** (diffuse) - Matte surfaces with realistic light scattering
- **Metal** (reflective) - Polished and brushed metal surfaces with configurable fuzziness
- **Dielectric** (glass/refractive) - Transparent materials with refraction and reflection
- **Diffuse Light** - Emissive materials for light sources
- **Isotropic** - Volumetric scattering for smoke and fog effects

### Textures
- **Solid Color** - Uniform color textures
- **Checker Texture** - Procedural checkerboard patterns
- **Image Texture** - Texture mapping from image files (JPEG, PNG, etc.)
- **Perlin Noise** - Procedural noise textures for natural-looking surfaces

### Primitives & Geometry
- **Spheres** - Basic sphere geometry with surface normals and UV mapping
- **Quads** - Quadrilateral primitives for building boxes and other shapes
- **Transformations** - Translation and Y-axis rotation for object positioning
- **Volumes** - Constant density medium for smoke, fog, and subsurface effects

### Rendering Features
- **BVH Acceleration** - Bounding Volume Hierarchy for fast ray-object intersection
- **Motion Blur** - Temporal sampling for moving objects
- **Importance Sampling** - PDF-based rendering for efficient light sampling
- **Depth of Field** - Configurable camera aperture and focus distance
- **Multi-threading** - Parallel rendering with OpenMP
- **Configurable Quality** - Adjustable samples per pixel and ray depth

### Camera
- Configurable viewport with aspect ratio
- Adjustable field of view (vertical FOV)
- Positionable camera (lookfrom, lookat, vup)
- Depth of field with defocus blur
- Background color/gradient support

### Output
- PPM image format

## Building

Requires:
- GCC compiler
- OpenMP library (libomp on macOS)

```bash
make
```

For debug build with symbols:
```bash
make debug
```

## Running

The program includes 10 pre-configured scenes demonstrating different features:

```bash
./GRTB <scene_number> > output.ppm
```

### Available Scenes

1. **Random bouncing spheres with motion blur** - Classic scene with animated spheres
2. **Two spheres with checkered texture** - Demonstrates procedural textures
3. **Earth globe with texture mapping** - Shows image-based textures
4. **Spheres with Perlin noise texture** - Procedural noise demonstration
5. **Colorful quads demonstration** - Quad primitives and transformations
6. **Simple scene with emissive light** - Basic lighting setup
7. **Cornell box with glass sphere** - Classic Cornell box with dielectric material
8. **Cornell box with smoke volumes** - Volumetric rendering demonstration
9. **Complex final scene (high quality)** - 800x800, 10000 samples (slow)
10. **Complex final scene (preview quality)** - 400x400, 250 samples (faster)

Example:
```bash
./GRTB 1 > bouncing_spheres.ppm
```

## Project Structure

### Core
- `main.c` - Scene selection and entry point
- `core/common.c/h` - Common definitions and utilities
- `core/memory.c/h` - Memory management helpers
- `core/grbt_stb_image.c/h` - Image loading wrapper

### Rendering
- `rendering/camera.c/h` - Camera and rendering logic
- `rendering/pdf.c/h` - Probability density functions for importance sampling

### Mathematics
- `math/vector3d.c/h` - 3D vector math operations
- `math/ray.c/h` - Ray representation and operations
- `math/interval.c/h` - Interval utilities for bounds
- `math/onb.c/h` - Orthonormal basis for coordinate systems

### Geometry & Acceleration
- `hittables/hittable.c/h` - Ray-object intersection interface
- `hittables/hittable_list.c/h` - Collection of hittable objects
- `hittables/sphere.c/h` - Sphere geometry implementation
- `hittables/quad.c/h` - Quadrilateral geometry
- `hittables/aabb.c/h` - Axis-aligned bounding boxes
- `hittables/bvh.c/h` - Bounding Volume Hierarchy acceleration structure

### Materials & Textures
- `materials/material.c/h` - Material types and scattering behavior
- `textures/texture.c/h` - Texture system and implementations
- `textures/perlin.c/h` - Perlin noise generation

### Scenes
- `scenes/` - Pre-configured scene definitions
  - `bouncing_spheres.c` - Motion blur demonstration
  - `checkered_spheres.c` - Checker texture example
  - `earth.c` - Image texture mapping
  - `perlin_spheres.c` - Procedural noise
  - `quads.c` - Quad primitives
  - `simple_light.c` - Emissive materials
  - `cornell_box.c` - Classic Cornell box
  - `cornell_smoke.c` - Volumetric rendering
  - `final_scene.c` - Complex scene with all features

## Reference

This project follows the structure and algorithms from:

**Ray Tracing in One Weekend Series** by Peter Shirley
- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)

## License

Educational project - see original book series for licensing details.
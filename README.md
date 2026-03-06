# GRTB - Ray Tracer in C

A C implementation of the ray tracer described in Peter Shirley's [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

## Features

- **Materials**: Lambertian (diffuse), Metal (reflective), and Dielectric (glass/refractive)
- **Primitives**: Sphere geometry with surface normals
- **Camera**: Configurable viewport with depth of field
- **Rendering**: Multi-threaded ray tracing with OpenMP
- **Output**: PPM image format

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

```bash
./GRTB > output.ppm
```

The program renders a scene with a ground plane and randomly generated spheres with different materials, outputting the result in PPM format to stdout.

## Project Structure

- `main.c` - Scene setup and entry point
- `camera.c/h` - Camera and rendering logic
- `ray.c/h` - Ray representation and operations
- `vector3d.c/h` - 3D vector math
- `hittable.c/h` - Ray-object intersection interface
- `sphere.c/h` - Sphere geometry implementation
- `material.c/h` - Material types and scattering
- `interval.c/h` - Interval utilities
- `memory.c/h` - Memory management helpers

## Reference

This project follows the structure and algorithms from:

**Ray Tracing in One Weekend** by Peter Shirley  
https://raytracing.github.io/books/RayTracingInOneWeekend.html

## License

Educational project - see original book for licensing details.

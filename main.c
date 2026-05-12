#include "common.h"
#include "scenes/scenes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s <scene_number>\n\n", program_name);
    fprintf(stderr, "Available scenes:\n");
    fprintf(stderr, "  1  - Random bouncing spheres with motion blur\n");
    fprintf(stderr, "  2  - Two spheres with checkered texture\n");
    fprintf(stderr, "  3  - Earth globe with texture mapping\n");
    fprintf(stderr, "  4  - Spheres with Perlin noise texture\n");
    fprintf(stderr, "  5  - Colorful quads demonstration\n");
    fprintf(stderr, "  6  - Simple scene with emissive light\n");
    fprintf(stderr, "  7  - Cornell box with glass sphere\n");
    fprintf(stderr, "  8  - Cornell box with smoke volumes\n");
    fprintf(stderr, "  9  - Complex final scene (high quality)\n");
    fprintf(stderr, "  10 - Complex final scene (preview quality)\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    int scene_number = atoi(argv[1]);

    switch (scene_number) {
        case 1:
            bouncingSpheres();
            break;
        case 2:
            checkeredSpheres();
            break;
        case 3:
            earth();
            break;
        case 4:
            perlinSpheres();
            break;
        case 5:
            quads();
            break;
        case 6:
            simple_light();
            break;
        case 7:
            cornell_box();
            break;
        case 8:
            cornell_smoke();
            break;
        case 9:
            final_scene(800, 10000, 40);
            break;
        case 10:
            final_scene(400, 250, 4);
            break;
        default:
            fprintf(stderr, "Error: Invalid scene number '%s'\n\n", argv[1]);
            print_usage(argv[0]);
            return 1;
    }

    return 0;
}
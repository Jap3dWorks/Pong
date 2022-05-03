#include "logger.h"
//#include "sandbox.h"
#include "Levels/blending_level.h"
#include "Levels/paint_with_shaders_level.h"
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

// https://project-awesome.org/terkelg/awesome-creative-coding#books
// TODO: Load models

int main(int argc, char** argv) {
    if (false) {
        BlendingLevel b_lvl;
        LOG_INFO("Blending level run");

        b_lvl.run();
    }

    else {
        auto s_lvl = PaintWithShades();
        LOG_INFO("Shading Level")
        s_lvl.run();
    }

    return 0;
}

// https://www.khronos.org/opengl/wiki/Built-in_Variable_(GLSL)
// https://www.youtube.com/watch?v=pXoDkHYmTxc&ab_channel=javidx9
#include "logger.h"
#include "sandbox.h"
#include "Levels/blending_level.h"
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

int main()
{
    LOG_DEBUG("Init app")
    LOG_WARNING("WARNING")

    BlendingLevel b_lvl;
    LOG_INFO("Blending level run");

    b_lvl.run();


    return 0;
}

// https://www.khronos.org/opengl/wiki/Built-in_Variable_(GLSL)
// https://www.youtube.com/watch?v=pXoDkHYmTxc&ab_channel=javidx9
#include "Utils/logger.h"
#include "Levels/load_model_level.h"

#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb/stb_image.h>

// https://project-awesome.org/terkelg/awesome-creative-coding#books
// TODO: Load models

int main(int argc, char **argv) {
    try {
        //    if (true) {
//        BlendingLevel b_lvl;
        LoadModelLevel b_lvl;
        LOG_INFO("Blending level run");

        b_lvl.run();
//    }

//    else {
//        auto s_lvl = PaintWithShades();
//        LOG_INFO("Shading Level");
//        s_lvl.run();
//    }
    }

    catch (const std::exception &e) {
        LOG_ERROR("ERROR: " << e.what());
    }
    catch (...) {
        LOG_ERROR("UNCATCHED EXCEPTION OCCURS!");
    }

    return 0;
}

// https://www.khronos.org/opengl/wiki/Built-in_Variable_(GLSL)
// https://www.youtube.com/watch?v=pXoDkHYmTxc&ab_channel=javidx9
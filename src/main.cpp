#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "logger.h"
#include "Levels/blending_level.h"

#include <iostream>

int main()
{
    BlendingLevel b_lvl;
    LOG_INFO("Blnding level run");

    b_lvl.run();

    return 0;
}

// https://www.khronos.org/opengl/wiki/Built-in_Variable_(GLSL)
// https://www.youtube.com/watch?v=pXoDkHYmTxc&ab_channel=javidx9
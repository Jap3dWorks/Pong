#include "logger.h"
#include "sandbox.h"
#include <iostream>

int main()
{
    // sandbox text
    auto vertex = quad_vertex();

    for (auto v: vertex)
    {
        std::cout << v.color.x << std::endl;
    }

    return 0;

//    BlendingLevel b_lvl;
//    LOG_INFO("Blnding level run");
//
//    b_lvl.run();
//
//
//    return 0;
}

// https://www.khronos.org/opengl/wiki/Built-in_Variable_(GLSL)
// https://www.youtube.com/watch?v=pXoDkHYmTxc&ab_channel=javidx9
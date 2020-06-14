#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "pongTypes.h"
#include "Actor.h"
#include "Level.h"

#include "logger.h"

#include <iostream>


int main()
{
    constexpr int case_id = 1;

    switch (case_id)
    {
    case 0:
        {
            LOG_DEBUG("case 0, test level")
            Pong::Level  lvl1;
            lvl1.run();
            break;
        }

    case 1:
        {
            LOG_DEBUG("case 1, test level")
            Pong::TestLevel t_lvl;
            t_lvl.run();
            break;
        }

    case 2:
        {
            LOG_DEBUG("case 2, test level")
            Pong::PBRLevel pbr_lvl;
            pbr_lvl.run();
            break;
        }

    default:
        break;

    }

    return 0;
}
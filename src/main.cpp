#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Movements.h"
#include "Actor.h"
#include "Level.h"

#include "logger.h"

#include <iostream>
#include <string>

int main()
{
//    constexpr int case_id = 0;

    int case_id;
    LOG_INFO("Enter 0, 1, or 2")
    std::cin >> case_id;

    switch (case_id)
    {
    case 0:
        {
            LOG_INFO("Case 0, Pong level")
            Pong::PongLevel  lvl1;
            lvl1.run();
            break;
        }

    case 1:
        {
            LOG_INFO("Case 1, test level")
            Pong::TestLevel t_lvl;
            t_lvl.run();
            break;
        }

    case 2:
        {
            LOG_INFO("Case 2, test level")
            Pong::PBRLevel pbr_lvl;
            pbr_lvl.run();
            break;
        }

    default:
        break;

    }

    return 0;
}
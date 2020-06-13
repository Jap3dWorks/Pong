#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "pongTypes.h"
#include "Actor.h"
#include "Scene.h"
#include "Level.h"

#include <iostream>


// TODO:
int main()
{

    constexpr int case_id = 1;

    switch (case_id)
    {
    case 0:
        {
            Pong::Level  lvl1;
            lvl1.run();
            break;
        }

    case 1:
        {
            std::cout << "case1, TestLevel\n";
            Pong::TestLevel t_lvl;
            t_lvl.run();
            break;
        }

    case 2:
        {
            Pong::PBRLevel pbr_lvl;
            pbr_lvl.run();
            break;
        }

    default:
        break;

    }

    return 0;
}
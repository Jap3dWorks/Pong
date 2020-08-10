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
    LOG_INFO("Case 1, test level");
    Pong::TestLevel t_lvl;
    LOG_INFO("test level run");

    t_lvl.run();

    return 0;
}
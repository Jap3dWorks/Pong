#ifndef RENDER_H
#define RENDER_H

#include "Movements.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// TODO: PBR materials
// TODO: buffered render
// TODO: postprocessings

namespace Pong {
    class Render
    {
    private:
        // last frame member
        float  _last_frame = 0;

        // draw scene class, SINGLETON
        GLFWwindow* _window;

        static Render* instance;

        static void _framebuffer_size_callback(GLFWwindow* window, int width, int height);

        Render();

    public:
        static const unsigned int SCR_WIDTH = 1280;
        static const unsigned int SCR_HEIGHT = 720;

        // time variables
        static float DeltaTime;

        void calculate_deltaTime();

        static Render* getInstance();

        GLFWwindow* getWindow();

        virtual ~Render();
    };
}

#endif // !RENDER_H
#ifndef RENDER_H
#define RENDER_H

#include "Movements.h"
#include "Shader.h"

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
        //Draw render buffers in this screen quad
        unsigned int _render_quad_vao{0};
        void _build_screen_quad();

        Shader* framebuffer_shader = nullptr;

        unsigned int _framebuffer{0};
        unsigned int _texture_color_buffer{0};
        void _config_frame_buffers();

        // Use for calculate delta time
        float  _last_frame_time = 0;

        GLFWwindow* _window;

        static Render* instance;

        static void _framebuffer_size_callback(GLFWwindow* window, int width, int height);

        // private constructor
        Render();

    public:
        /**Binds framebuffer, in this buffer the scene is prerendered.*/
        void bind_framebuffer() const;
        /**Draws framebuffer in default buffer*/
        void draw_framebuffer();

        static const unsigned int SCR_WIDTH = 1280;
        static const unsigned int SCR_HEIGHT = 720;
        static constexpr float Z_NEAR = 0.1f;
        static constexpr float Z_FAR = 10000.f;

        // time variables
        static float DeltaTime;

        void calculate_deltaTime();

        static Render* getInstance();

        GLFWwindow* getWindow();

        virtual ~Render();
    };
}

#endif // !RENDER_H
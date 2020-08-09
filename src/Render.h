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
        /**Draw render buffers in this screen quad*/
        unsigned int _render_quad_vao{0};
        void _build_screen_quad();

        Shader framebuffer_shader{
                "../shaders/franebuffer_screen_V.glsl",
                "../shaders/framebuffer_screen_F.glsl"};

        unsigned int _framebuffer{0};
        unsigned int _texture_color_buffer{0};
        void _config_frame_buffers();

        // TODO: framebuffer render operations,
        //  render in framebuffer and draw later in _render_quad_vao
        // last frame member
        float  _last_frame = 0;

        // draw scene class, SINGLETON
        GLFWwindow* _window;

        static Render* instance;

        static void _framebuffer_size_callback(GLFWwindow* window, int width, int height);

        Render();

    public:
        void bind_framebuffer() const;
        void draw_framebuffer();
        void swap_buffers_poll_events();

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
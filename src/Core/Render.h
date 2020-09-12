#ifndef RENDER_H
#define RENDER_H

#include "Movements.h"
#include "../Graphic/Shader.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// TODO: PBR materials
// TODO: postprocessings
// TODO: UBO for scene lights
namespace Pong {
    class ACamera;
    class Scene;

    enum class RenderLayer: unsigned int {
        NO_LAYER=0,
        BASE=1,
        SKY_BOX=2,
        BLENDING=4,};

    RenderLayer operator|(const RenderLayer& lrl, const RenderLayer& rrl);
    RenderLayer operator&(const RenderLayer& lrl, const RenderLayer& rrl);

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

        // uniform buffer, stores projection matrix and view matrix
        // TODO: here, configure camera uniform buffers
        /**uniform buffer view matrices is bind in binding point 0*/
        unsigned int _ubo_view;
        void _create_ubo_view_matrices();

        unsigned int _ubo_lights;
        void _create_ubo_lights();

        // Use for calculate delta time
        float  _last_frame_time = 0;

        GLFWwindow* _window;

        static Render* instance;

        static void _framebuffer_size_callback(GLFWwindow* window, int width, int height);

        // private constructor
        Render();

    public:
        /**Binds framebuffer, in this buffer the scene is prerendered.*/
        static void bind_framebuffer(unsigned int in_framebuffer=0) ;
        /**Draws framebuffer in default buffer*/
        void draw_framebuffer();

        void update_ubo_view(ACamera* camera) const;
        void update_ubo_lights(Scene* scene) const;

        static const unsigned int SCR_WIDTH = 1280;
        static const unsigned int SCR_HEIGHT = 720;
        static constexpr float Z_NEAR = 0.1f;
        static constexpr float Z_FAR = 10000.f;

        [[nodiscard]] unsigned int get_framebuffer() const {return _framebuffer;}

        // time variables
        static float DeltaTime;

        void calculate_deltaTime();

        static Render* getInstance();

        GLFWwindow* getWindow();

        virtual ~Render();

        RenderLayer first_pass_renderlayers[2] = {RenderLayer::BASE,
                                                  RenderLayer::SKY_BOX};

        /**
         Configure open gl enables using bitwise operations
         1-> glEnable(GL_DEPTH_TEST)
         2-> glEnable(GL_BLEND)
         4-> glEnable(GL_CULL_FACE)
         8-> glEnable(GL_PROGRAM_POINT_SIZE)
         The upper list must be read as bits, e.g 4 -> 00000100
         You can modigi gl_enables_config attribute to config openGL enables, after edit
         gl_enables_config you should tu call update_enables to make changes effective.*/
        unsigned char gl_enables_config = 7;
        /**Make gl_enables_config configuration effective*/
        void update_enables() const;
    };
}

bool any(const Pong::RenderLayer& rlay);


#endif // !RENDER_H
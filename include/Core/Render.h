#ifndef RENDER_H
#define RENDER_H

#include "Core/core_vals.h"
#include "Core/Movements.h"
#include "Core/Shader.h"

#include <cstdint>
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// TODO: PBR materials
// TODO: postprocessing

namespace Pong {
    class ACamera;
    class Scene;

    enum class RenderLayer: unsigned int {
        NO_LAYER=0,
        BASE=1,
        SKY_BOX=2,
        BLENDING=4
    };

    enum class UboLayouts : unsigned int {
        VIEW_MATRICES=0,
        LIGHTS=1,
        FRAME_DATA=2
    };

    RenderLayer operator|(const RenderLayer& lrl, const RenderLayer& rrl);
    RenderLayer operator&(const RenderLayer& lrl, const RenderLayer& rrl);

    class Render {
    private:
        uint32_t _frame=0;
        float _fps=0.f;
        uint32_t _time;

        unsigned int _render_quad_vao{0};
        Shader* framebuffer_shader = nullptr;

        unsigned int _framebuffer{0};
        unsigned int _texture_color_buffer{0};
        unsigned int _ubo_view;
        unsigned int _ubo_lights;
        unsigned int _ubo_frame_data;

        // Use for calculate delta time
        float  _last_frame_time = 0;

        GLFWwindow* _window;
        static Render* instance;

    private:
        //Draw render buffers in this screen quad
        void _build_screen_quad();
        void _config_frame_buffers();

        // uniform buffer, stores projection matrix and view matrix
        /**uniform buffer view matrices is bind in binding point 0*/
        void _create_ubo_view_matrices();

        void _create_ubo_lights();
        static void _framebuffer_size_callback(GLFWwindow* window, int width, int height);

        void _create_ubo_frame_data();
        // private constructor
        Render();

    public:
        static const unsigned int SCR_WIDTH = 1280;
        static const unsigned int SCR_HEIGHT = 720;
        static constexpr float Z_NEAR = 0.1f;
        static constexpr float Z_FAR = 10000.f;

        // time variables
        static float DeltaTime;

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

    public:
        /**Binds framebuffer, in this buffer the scene is prerendered.*/
        static void bind_framebuffer(unsigned int in_framebuffer=0) ;
        /**Draws framebuffer in default buffer*/
        void draw_framebuffer();

        void update_ubo_view(ACamera* camera) const;
        void update_ubo_lights(Scene* scene) const;
        void update_frame_data() const;

        _P_NODISCARD unsigned int get_framebuffer() const {return _framebuffer;}

        void calculate_deltaTime();

        static Render* getInstance();

        GLFWwindow* getWindow();

        virtual ~Render();

        /**Make gl_enables_config configuration effective*/
        void update_enables() const;


        void clear_frame() {_frame=0;}

        _P_NODISCARD const auto& get_frame() const{
            return _frame;}


    };
}

bool any(const Pong::RenderLayer& rlay);


#endif // !RENDER_H
#ifndef RENDER_H
#define RENDER_H

#include "Pong/Core/core_vals.h"
#include "Pong/Core/movements.h"
#include "Pong/Core/shader.h"
#include "Pong/Core/actor.h"
#include "Pong/Core/lights.h"

#include <cstdint>
#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#define MAX_UINT_8 255

// TODO: PBR materials
// TODO: postprocessing

namespace Pong {
    enum class RenderLayer: uint32_t {
        NO_LAYER=0,
        BASE=1,
        SKY_BOX=2,
        BLENDING=4
    };

    _P_INLINE RenderLayer operator|(const Pong::RenderLayer& lrl, const Pong::RenderLayer& rrl) {
        return static_cast<Pong::RenderLayer>(
                static_cast<uint32_t>(lrl) | static_cast<uint32_t>(rrl));
    }

    _P_INLINE RenderLayer operator&(const Pong::RenderLayer& lrl, const Pong::RenderLayer& rrl) {
        return static_cast<Pong::RenderLayer>(
                static_cast<uint32_t>(lrl) & static_cast<uint32_t>(rrl));
    }

    _P_INLINE bool any(const Pong::RenderLayer& rlay) {
        return rlay != Pong::RenderLayer::NO_LAYER;
    }

    enum class UboLayouts : GLuint {
        VIEW_MATRICES=0,
        LIGHTS=1,
        RUNTIME_DATA=2,  // frame, fps, time, delta_time
        RENDER_DATA=3,
    };

    class Render {
    private:
        struct RuntimeData {
            double delta_time = 0;
            double time = glfwGetTime();
            GLuint frame_counter = 0;
            float fps = 0.f;
        } _runtime_data;

        struct RenderData {
            uint32_t width=1280;
            uint32_t height=720;
            float z_near = 0.1;
            float z_far = 10000.f;
            float aspect = float(width) / float(height);
        } _render_data;

        uint32_t _render_quad_vao = 0;

        std::unique_ptr<Shader> framebuffer_shader =
                std::unique_ptr<Shader>(nullptr);

        uint32_t _framebuffer = 0;
        uint32_t _texture_color_buffer = 0;
        uint32_t _ubo_view = 0;
        uint32_t _ubo_lights = 0;
        uint32_t _ubo_runtime_data = 0;
        uint32_t _ubo_render_data = 0;

        GLFWwindow* _window;
        _P_STATIC _P_INLINE Render* instance = nullptr;

    public:
        _P_CONSTEXPR _P_STATIC _P_INLINE GLuint MAX_DIRECTIONAL_LIGHTS = 4;
        _P_CONSTEXPR _P_STATIC _P_INLINE GLuint MAX_POINT_LIGHTS = 32;

        RenderLayer first_pass_render_layers[2] =
                {RenderLayer::BASE,
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

    private:
        //Draw Render buffers in this screen quad
        _P_INLINE void _build_screen_quad() {
            float quad_vertex[] = {
                    -1.0f, 1.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f,
                    1.0f, -1.0f, 1.0f, 0.0f,
                    -1.0f, 1.0f, 0.0f, 1.0f,
                    1.0f, -1.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 1.0f, 1.0f
            };

            unsigned int cubeVBO;
            glGenVertexArrays(1, &_render_quad_vao);
            glGenBuffers(1, &cubeVBO);
            glBindVertexArray(cubeVBO);
            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex), &quad_vertex, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                                  (void*)nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                                  (void*)(2 * sizeof(float)));
        }

        _P_INLINE void _config_frame_buffers() {
            LOG_DEBUG("Config framebuffer")
            glGenFramebuffers(1, &_framebuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

            // create color attachment texture
            glGenTextures(1, &_texture_color_buffer);
            glBindTexture(GL_TEXTURE_2D, _texture_color_buffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                         (GLint) _render_data.width, (GLint) _render_data.height,
                         0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_2D, _texture_color_buffer, 0);

            // Render buffer object
            unsigned int rbo;
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                                  (GLint) _render_data.width, (GLint) _render_data.height);

            // attach renderbuffer with framebuffer
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                      GL_RENDERBUFFER, rbo);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }

        // uniform buffer, stores projection matrix and view matrix
        /**uniform buffer view matrices is bind in binding point 0*/
        _P_INLINE void _create_ubo_view_matrices() {
            assert(_ubo_view == 0);

            unsigned int ubo_size = (2 * sizeof(glm::mat4)) +
                                     sizeof(glm::vec4) +
                                     sizeof(float);

            glGenBuffers(1, &_ubo_view);
            glBindBuffer(GL_UNIFORM_BUFFER, _ubo_view);
            glBufferData(GL_UNIFORM_BUFFER, ubo_size, nullptr, GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            // bind ubo matrices to index 0
            glBindBufferRange(GL_UNIFORM_BUFFER,
                              static_cast<uint32_t>(UboLayouts::VIEW_MATRICES),
                              _ubo_view, 0, ubo_size);
        }

        _P_INLINE void _create_ubo_lights() {
            assert(_ubo_lights == 0);

            GLuint size_buffer =
                    (sizeof(DirectionalLight) * MAX_DIRECTIONAL_LIGHTS) +
                    (sizeof(PointLight) * MAX_POINT_LIGHTS) +
                    sizeof(decltype(MAX_DIRECTIONAL_LIGHTS)) +
                    sizeof(decltype(MAX_POINT_LIGHTS));

            glGenBuffers(1, &_ubo_lights);
            glBindBuffer(GL_UNIFORM_BUFFER, _ubo_lights);

            // https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL

            glBufferData(GL_UNIFORM_BUFFER,  size_buffer,
                         nullptr, GL_STATIC_DRAW);

            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            // bind to index 1
            glBindBufferRange(GL_UNIFORM_BUFFER,
                              static_cast<GLuint>(UboLayouts::LIGHTS),
                              _ubo_lights, 0, size_buffer);
        }

        _P_INLINE void _create_ubo_runtime_data() {
            assert(_ubo_runtime_data == 0);

            uint32_t size_buffer = sizeof(_runtime_data);

            glGenBuffers(1, &_ubo_runtime_data);

            glBindBuffer(GL_UNIFORM_BUFFER, _ubo_runtime_data);
            // set projection
            glBufferData(GL_UNIFORM_BUFFER, size_buffer,
                         nullptr, GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            glBindBufferRange(GL_UNIFORM_BUFFER,
                              static_cast<uint32_t>(UboLayouts::RUNTIME_DATA),
                              _ubo_runtime_data, 0, size_buffer);
        }

        _P_INLINE void _create_ubo_render_data() {
            assert(_ubo_render_data == 0);

            uint32_t size_buffer = sizeof(_render_data);

            glGenBuffers(1, &_ubo_render_data);
            glBindBuffer(GL_UNIFORM_BUFFER, _ubo_render_data);
            glBufferData(GL_UNIFORM_BUFFER, size_buffer,
                         nullptr, GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            glBindBufferRange(GL_UNIFORM_BUFFER,
                              static_cast<uint32_t>(UboLayouts::RENDER_DATA),
                              _ubo_render_data, 0, size_buffer);
        }

        _P_STATIC void _framebuffer_size_callback(GLFWwindow* window,
                                               int width,
                                               int height) {
            // make sure the viewport matches the new window dimensions; note that width and
            // height will be significantly larger than specified on retina displays.
            glViewport(0, 0, width, height);
        }

        // private constructor
        Render() {
            //glfw initialize and configure
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            // glfw window creation

            _window = glfwCreateWindow((int) _render_data.width,
                                       (int) _render_data.height,
                                       "LearnOpenGL",
                                       nullptr,
                                       nullptr);

            if (!_window) {
                LOG_ERROR("Failed to creat GLFW window");
                glfwTerminate();
            }

            glfwMakeContextCurrent(_window);

            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            // glad: load all opengl functions pointers
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                LOG_ERROR("Failed to initialize GLAD");
                return;
            }

            glfwSetFramebufferSizeCallback(_window, _framebuffer_size_callback);

            _config_frame_buffers();
            _build_screen_quad();

            // return default framebuffer
            // glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // glEnable operations
            update_enables();

            // framebuffer shaders
            framebuffer_shader = std::make_unique<Shader>(
                    Shader("./Shaders/framebuffer_screen.glsl"));

            _create_ubo_view_matrices();
            _create_ubo_lights();
            _create_ubo_runtime_data();
            _create_ubo_render_data();

            update_ubo_render_data();
        }

    public:
        /**Binds framebuffer, in this buffer the Scene is prerendered.*/
        _P_STATIC _P_INLINE void bind_framebuffer(unsigned int in_framebuffer=0) {
            glBindFramebuffer(GL_FRAMEBUFFER, in_framebuffer);
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        /**Draws framebuffer in default buffer,
         * Also Updates frame counter and deltatime*/
        _P_INLINE void draw_framebuffer() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);
            glClearColor(1.0, 1.0, 1.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            framebuffer_shader->use();
            glBindVertexArray(_render_quad_vao);
            glBindTexture(GL_TEXTURE_2D, _texture_color_buffer);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(_window);
            glfwPollEvents();

            _runtime_data.frame_counter++;
            update_time_data();
        }

        _P_INLINE void update_ubo_view(ACamera* camera) const {
            glBindBuffer(GL_UNIFORM_BUFFER, _ubo_view);

            uint8_t buffer_offset = 0;
            float fov = glm::radians(camera->Zoom);

            // set projection
            glBufferSubData(GL_UNIFORM_BUFFER,
                            buffer_offset,
                            sizeof(glm::mat4),
                            glm::value_ptr(
                                    glm::perspective(fov,
                                                     (float)_render_data.width / (float)_render_data.height,
                                                     _render_data.z_near,_render_data.z_far)));
            buffer_offset += sizeof(glm::mat4);

            // set view
            glBufferSubData(GL_UNIFORM_BUFFER,
                            buffer_offset,
                            sizeof(glm::mat4),
                            glm::value_ptr(camera->get_view_matrix()));
            buffer_offset += sizeof(glm::mat4);

            // view pos
            glBufferSubData(GL_UNIFORM_BUFFER,
                            buffer_offset,
                            sizeof(glm::vec4),
                            glm::value_ptr(camera->Position));

            buffer_offset += sizeof(glm::vec4);

            // Fov (Zoom)
            glBufferSubData(GL_UNIFORM_BUFFER,
                            buffer_offset,
                            sizeof(float),
                            (void*)&fov
                            );

            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        _P_INLINE void update_ubo_lights (
                const std::vector<DirectionalLight>& directional_lights={},
                const std::vector<PointLight>& point_lights={}
                ) _P_CONST noexcept {

            uint32_t buffer_offset = 0;

            assert (directional_lights.size() <= MAX_DIRECTIONAL_LIGHTS);
            assert (point_lights.size() <= MAX_POINT_LIGHTS);

            glBindBuffer(GL_UNIFORM_BUFFER, _ubo_lights);

            // set directional and point
            glBufferSubData(GL_UNIFORM_BUFFER,
                            buffer_offset,
                            sizeof(DirectionalLight) * MAX_DIRECTIONAL_LIGHTS,
                            (void*)directional_lights.data());

            buffer_offset += sizeof(DirectionalLight) * MAX_DIRECTIONAL_LIGHTS;

            glBufferSubData(GL_UNIFORM_BUFFER,
                            buffer_offset,
                            sizeof(PointLight) * MAX_POINT_LIGHTS,
                            (void*)point_lights.data());

            buffer_offset += sizeof(PointLight) * MAX_POINT_LIGHTS;


            // arrays sizes
            auto directional_lights_count =
                    static_cast<decltype(MAX_DIRECTIONAL_LIGHTS)>
                    (directional_lights.size());

            glBufferSubData(GL_UNIFORM_BUFFER,
                            buffer_offset,
                            sizeof(decltype(directional_lights_count)),
                            (void*)&directional_lights_count);

            buffer_offset += sizeof(decltype(directional_lights_count));

            auto point_lights_count =
                    static_cast<decltype(MAX_POINT_LIGHTS)>(point_lights.size());

            glBufferSubData(GL_UNIFORM_BUFFER,
                            buffer_offset,
                            sizeof(decltype(point_lights_count)),
                            (void*)&point_lights_count
                            );
        }

        _P_INLINE void update_ubo_runtime_data() const {
            assert(_ubo_runtime_data != 0);

            glBindBuffer(GL_UNIFORM_BUFFER, _ubo_runtime_data);

            uint32_t offset_size = 0;

            glBufferSubData(GL_UNIFORM_BUFFER,
                            offset_size,
                            sizeof(_runtime_data),
                            &_runtime_data);

            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        _P_INLINE void update_ubo_render_data() const {
            assert(_ubo_runtime_data != 0);

            glBufferSubData(GL_UNIFORM_BUFFER,
                            0,
                            sizeof(_render_data),
                            &_render_data);
        }

        _P_NODISCARD _P_INLINE auto get_framebuffer() const
            {return _framebuffer;}

        _P_INLINE void update_time_data() {
            auto current_frame = glfwGetTime();
            _runtime_data.delta_time = current_frame - _runtime_data.time;
            _runtime_data.time = current_frame;
        }

        _P_STATIC _P_INLINE Render* get_instance() {
            if (!Render::instance) {
                Render::instance = new Render;
            }
            return Render::instance;
        }

        auto get_window() {
            return _window;
        }

        virtual ~Render() {
            glfwTerminate();
        }

        /**Make gl_enables_config configuration effective*/
        _P_INLINE void update_enables() const {
            // configure global opengl state
            if (gl_enables_config & 1){
                LOG_DEBUG("Active GL_DEPTH_TEST.")
                glEnable(GL_DEPTH_TEST);
            }
            if(gl_enables_config & 2) {
                LOG_DEBUG("Active GL_BLEND, GL_ONE_MINUS_SRC_ALPHA.")
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            if (gl_enables_config & 4) {
                LOG_DEBUG("Active GL_CULL_FACE.")
                glEnable(GL_CULL_FACE);
            }
            if (gl_enables_config & 8) {
                LOG_DEBUG("Active GL_PROGRAM_POINT_SIZE.")
                glEnable(GL_PROGRAM_POINT_SIZE);
            }
        }

        _P_INLINE void clear_frame_count() noexcept { _runtime_data.frame_counter=0;}

        _P_NODISCARD const auto& get_frame_count() const noexcept {
            return _runtime_data.frame_counter;
        }

        _P_NODISCARD _P_INLINE const auto& get_delta_time() const noexcept {
            return _runtime_data.delta_time;
        }

        _P_NODISCARD _P_INLINE const auto& get_runtime_Data() const noexcept {
            return _runtime_data;
        }

        _P_NODISCARD _P_INLINE const auto& get_render_data() const noexcept {
            return _render_data;
        }
    };
}


#endif // !RENDER_H
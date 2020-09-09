#include "Render.h"
#include "../logger.h"

#include "Actor.h"

#include <iostream>


float Pong::Render::DeltaTime = 0;

void Pong::Render::calculate_deltaTime()
{
    float current_frame = glfwGetTime();
    Pong::Render::DeltaTime = current_frame - _last_frame_time;
    _last_frame_time = current_frame;
}
Pong::Render* Pong::Render::instance = 0;

Pong::Render::Render()
{
    //glfw initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    _window = glfwCreateWindow(SCR_WIDTH,
        SCR_HEIGHT,
        "LearnOpenGL",
        nullptr,
        nullptr);

    if (!_window)
    {
        LOG_ERROR("Failed to creat GLFW window");
        glfwTerminate();
    }

    glfwMakeContextCurrent(_window);

    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all opengl functions pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        return;
    }

    glfwSetFramebufferSizeCallback(_window, _framebuffer_size_callback);

    _config_frame_buffers();
    _build_screen_quad();
    LOG_DEBUG("Framebuffers created")

    // return default framebuffer
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // glEnable operations
    update_enables();

    // framebuffer shaders
    framebuffer_shader = new Shader(
            "./Shaders/framebuffer_screen_V.glsl",
            "./Shaders/framebuffer_screen_F.glsl");

    _create_ubo_view_matrices();

}

void Pong::Render::update_enables() const {
    // configure global opengl state
    if (gl_enables_config & 1){
        LOG_DEBUG("Active GL_DEPTH_TEST.")
        glEnable(GL_DEPTH_TEST);
    }
    if(gl_enables_config & 2)
    {
        LOG_DEBUG("Active GL_BLEND, GL_ONE_MINUS_SRC_ALPHA.")
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    if (gl_enables_config & 4)
    {
        LOG_DEBUG("Active GL_CULL_FACE.")
        glEnable(GL_CULL_FACE);
    }
    if (gl_enables_config & 8)
    {
        LOG_DEBUG("Active GL_PROGRAM_POINT_SIZE.")
        glEnable(GL_PROGRAM_POINT_SIZE);
    }
}

void  Pong::Render::_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

Pong::Render* Pong::Render::getInstance()
{
    if (!Render::instance)
    {
        Render::instance = new Render;
    }
    return Render::instance;
}

GLFWwindow* Pong::Render::getWindow()
{
    return _window;
}

Pong::Render::~Render()
{
    delete framebuffer_shader;
    glfwTerminate();
}

void Pong::Render::_build_screen_quad() {
    float quad_vertex[] =
            {
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

void Pong::Render::_config_frame_buffers() {
    LOG_DEBUG("Config framebuffer")
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    // create color attachment texture
    glGenTextures(1, &_texture_color_buffer);
    glBindTexture(GL_TEXTURE_2D, _texture_color_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT,
            0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, _texture_color_buffer, 0);

    // render buffer object
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                          SCR_WIDTH, SCR_HEIGHT);

    // attach renderbuffer with framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}

void Pong::Render::bind_framebuffer(unsigned int in_framebuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, in_framebuffer);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Pong::Render::draw_framebuffer()
{
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
}

void Pong::Render::_create_ubo_view_matrices()
{
    glGenBuffers(1, &_ubo_matrices);
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo_matrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // bind ubo matrices to index 0
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ubo_matrices, 0, 2 * sizeof(glm::mat4));
}

void Pong::Render::update_ubo_view_matrices(Pong::ACamera *camera) const {
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo_matrices);
    // set projection
    glBufferSubData(GL_UNIFORM_BUFFER,
                    0, sizeof(glm::mat4),
                    glm::value_ptr(
                            glm::perspective(glm::radians(camera->Zoom),
                                             (float) Pong::Render::SCR_WIDTH / (float) Pong::Render::SCR_HEIGHT,
                                             Pong::Render::Z_NEAR,
                                             Pong::Render::Z_FAR)));

    // set view
    glBufferSubData(GL_UNIFORM_BUFFER,
                    sizeof(glm::mat4), sizeof(glm::mat4),
                    glm::value_ptr(camera->get_view_matrix()));

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Pong::RenderLayer operator|(const Pong::RenderLayer& lrl, const Pong::RenderLayer& rrl){
return static_cast<Pong::RenderLayer>(
        static_cast<unsigned int>(lrl) | static_cast<unsigned int>(rrl));
}

Pong::RenderLayer operator&(const Pong::RenderLayer& lrl, const Pong::RenderLayer& rrl){
return static_cast<Pong::RenderLayer>(
        static_cast<unsigned int>(lrl) & static_cast<unsigned int>(rrl));
}

bool any(const Pong::RenderLayer& rlay){
    return rlay != Pong::RenderLayer::NO_LAYER;
}



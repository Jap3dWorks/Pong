#include "Render.h"
#include "logger.h"



float Pong::Render::DeltaTime = 0;

void Pong::Render::calculate_deltaTime()
{
    float current_frame = glfwGetTime();
    Pong::Render::DeltaTime = current_frame - _last_frame;
    _last_frame = current_frame;
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
        NULL,
        NULL);

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

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

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
    glfwTerminate();
}

void Pong::Render::_build_render_quad() {
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

    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    // create color attachment texture
    unsigned int _texture_color_buffer;
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
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    // attach renderbuffer with framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    // return default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

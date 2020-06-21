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

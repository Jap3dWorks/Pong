//
// Created by Jordi on 6/30/2022.
//

#ifndef GL_TEST_OUTPUTS_H
#define GL_TEST_OUTPUTS_H

#include <GLFW/glfw3.h>
#include "Pong/core/process_input.h"
#include "Utils/type_conditions.h"
#include "Pong/core/exceptions.h"
#include <concepts>
#include <memory>
#include <string>

// TODO: move here _window logic

#define OutputTemplate(_TEMP_ARGS_) template<std::derived_from<Output> T, _TEMP_ARGS_>


namespace pong {
    class Output {

    protected:
        std::string _name{};
        explicit Output(std::string name): _name(std::move(name)) {}

    public:
        virtual void update() = 0;
        virtual ~Output() = default;

        OutputTemplate(typename ...Args)
        friend T *create_output(std::string name, Args ...args);

        [[nodiscard]] std::string get_name() const noexcept {
            return _name;
        }
    };

    class WindowOutput: public Output {
    public:
        using Window = GLFWwindow;
//        using WindowUniquePtr = std::unique_ptr<Window>;
        using WindowPtr = Window*;
        using resize_callback = GLFWframebuffersizefun;

    private:
        uint32_t _width{};
        uint32_t _height{};
        WindowPtr _window = nullptr;

    public:

    private:
        WindowOutput(std::string name,
                     uint32_t width,
                     uint32_t height,
                     ProcessInput *_input = nullptr) :
                Output(std::move(name)),
                _width(width),
                _height(height) {

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            _window = glfwCreateWindow((int) _width,
                                       (int) _height,
                                       _name.c_str(),
                                       nullptr,
                                       nullptr);

            if (!_window) {
                LOG_ERROR("Failed to creat GLFW _window");
                glfwTerminate();
                throw CreateOutputWindowError(
                        "FAIL During WindowOutput creation."
                        );
            }

            glfwMakeContextCurrent(_window);
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        void set_size_callback(resize_callback callback) {
            glfwSetFramebufferSizeCallback(_window, callback);
        }

    public:
        void update() override {
            glfwSwapBuffers(_window);
        }

        [[nodiscard]] WindowPtr get_window() const noexcept {
            return _window;
        }

//        ~WindowOutput() override {
//            glfwTerminate();
//    }

    };

    class FileOutput : public Output {

    private:
        explicit FileOutput(std::string name) :
                Output(std::move(name)) {}

    public:
        void update() override {}

    };
}

#endif //GL_TEST_OUTPUTS_H

//
// Created by Jordi on 7/25/2022.
//

#ifndef GL_TEST_ENGINE_H
#define GL_TEST_ENGINE_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Pong/render/render.h"
#include "Pong/core/outputs.h"
#include <GLFW/glfw3.h>

namespace Pong {
    class Output;
    class ProcessInput;
    class Render;
    class Map;
}

namespace Pong {
    class Engine {
    public:
        using OutputPtr = std::unique_ptr<Output>;
        using OutputMap = std::unordered_map<std::string, OutputPtr>;

        using InputPtr = std::unique_ptr<ProcessInput>;
        using InputMap = std::unordered_map<std::string, InputPtr>;

        using RenderPtr = std::unique_ptr<Render>;
//        using RenderMap = std::unordered_map<std::string, RenderPtr>;

        using ScenePtr = std::unique_ptr<Map>;
        using SceneMap = std::unordered_map<std::string, ScenePtr>;


        using EngineUniquePtr = std::unique_ptr<Engine>;

    private:
        inline static EngineUniquePtr _instance{};

    protected:
        OutputMap _outputs{};
        InputMap _inputs{};
        SceneMap _scenes{};
        RenderPtr _render=nullptr;

    private:
        Engine() {
            glfwInit();
            _render = std::make_unique<Render>();
        };

    public:
        ~Engine() {
            glfwTerminate();
        }

        inline static Engine* get_instance() {
            if (!_instance) {
                _instance = std::unique_ptr<Engine>(new Engine);
            }
            return _instance.get();
        }

        void add_output(const std::string& name, OutputPtr&& output) {
            _outputs[name] = std::move(output);
        }
    };

    OutputTemplate(typename ...Args)
    inline T* create_output(std::string name, Args ...args) {
        auto output = new T(std::move(name), std::forward<Args>(args)...);

        Engine::get_instance()->add_output(
                output->get_name(),
                std::unique_ptr<T>(output)
                );

        return output;
    }

    inline Render* create_render(const std::string& name) {

    }

    void run() {}

}
#endif //GL_TEST_ENGINE_H

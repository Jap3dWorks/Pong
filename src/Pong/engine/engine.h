//
// Created by Jordi on 7/25/2022.
//

#ifndef PONG_SRC_PONG_ENGINE_ENGINE_H_
#define PONG_SRC_PONG_ENGINE_ENGINE_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Pong/render/render.h"
#include "Pong/core/outputs.h"
#include "Pong/engine/file_inspector.h"
#include "Pong/registers/reg_data_controller.h"

#include <GLFW/glfw3.h>

namespace pong {
    class Output;
    class ProcessInput;
    class Render;
    class Map;
}

namespace pong::engine {

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
        inline static EngineUniquePtr instance_{};

    protected:
        OutputMap outputs{};
        InputMap inputs{};
        SceneMap scenes{};
        RenderPtr render=nullptr;

        AssetInspector asset_inspector{};
        BufferedRegister buffered_register{};

    private:
        Engine() {
            glfwInit();
            render = std::make_unique<Render>();
        };

    public:
        ~Engine() {
            glfwTerminate();
        }

        inline static Engine* get_instance() {
            if (!instance_) {
                instance_ = std::unique_ptr<Engine>(new Engine);
            }
            return instance_.get();
        }

        void add_output(const std::string& name, OutputPtr&& output) {
            outputs[name] = std::move(output);
        }

        void run() {}

        void setup() {
            // inspect asset and seg asset ids
            asset_inspector.collect_asset_files();
            asset_inspector.collect_asset_data();
        }

        const auto& get_data_location_reg() {
            return asset_inspector.get_data_location_reg();
        }

        auto& get_buffered_register() {
            return buffered_register;
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


}
#endif //PONG_SRC_PONG_ENGINE_ENGINE_H_

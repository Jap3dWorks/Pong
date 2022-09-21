//
// Created by Jordi on 7/25/2022.
//

#ifndef PONG_SRC_PONG_ENGINE_ENGINE_H_
#define PONG_SRC_PONG_ENGINE_ENGINE_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include <memory>
#include <GLFW/glfw3.h>

#include "Pong/render/render.h"
#include "Pong/core/outputs.h"
#include "Pong/serializer/descriptors.h"
#include "Pong/engine/asset_inspector.h"
#include "Pong/registers/reg_data_controller.h"

#include "Pong/components/component_hasher.h"


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

        map::Map map_;

        inline static EngineUniquePtr instance_{};
        bool close_ = false;

    protected:
        OutputMap outputs{};
        InputMap inputs{};
        SceneMap scenes{};
        RenderPtr render=nullptr;

        ContentInspector asset_inspector{};  // inspect asset files
        BufferedRegisters buffered_register{}; // geos and data


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

        void run() {
            while(!close_) {

            }
        }

        void setup() {
            // inspect asset and seg asset ids
            asset_inspector.collect_files();
            asset_inspector.collect_asset_data();
            asset_inspector.collect_map_data();

            auto map_location = asset_inspector.get_data_location_reg().map_reg.get_type<MapDtLocation>(RegId{1});

            map_ = load_location(map_location);

            // register components
            using Range = boost::mpl::range_c<uint32_t, 1, COUNTER_READ(serializer::component_tag) + 1>;

            boost::mpl::for_each<Range>(
                [&]<typename I>(I i) constexpr -> void {
//                    datacontroller.reg_type<typename serializer::components_collected<I::value>::type>();
                }
            );

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

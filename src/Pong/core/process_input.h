#ifndef PROCESS_INPUT_H
#define PROCESS_INPUT_H

#include "Pong/core/movements.h"
#include "Pong/core/actor.h"
#include "Utils/action_event.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <map>
#include <utility>

namespace Pong {

    /**
    * Functor class useful to manage the inputs of an Actor object.
    */
    class ProcessInput {
    public:
        using Window = GLFWwindow;
        using EventMap = std::map<int32_t, Event<void(*)()>>;
        using CursorCallback = void (*)(Window*, double, double);
        using ScrollCallback = void (*)(Window*, double, double);

    private:
//        inline static ProcessInput* _instance;
        Window* _window = nullptr;

    public:
        EventMap _event_map{};

    public:
        /**
         * Constructor receives a pointer of template class,
         *  also a map with the relations (Key, Movements::_anyMove_)
         * @param _window:
         */
    public:
        explicit ProcessInput(
                Window *window
        ) : _window(window) {}

    void set_cursor_callback(CursorCallback _callback) noexcept {
        glfwSetCursorPosCallback(_window, _callback);
        }

    void set_scroll_callback(ScrollCallback _callback) noexcept {
        glfwSetScrollCallback(_window, _callback);
        }

//        static inline ProcessInput *get_instance(
//                GLFWwindow *_window
//        ) {
//            if (_instance == nullptr) {
//                _instance = new ProcessInput(_window);
//            }
//            return _instance;
//        }

        void update() const {
            for (auto &pair: _event_map) {
                if (glfwGetKey(_window, pair.first) == GLFW_PRESS) {
                    pair.second();
                }
            }
        }

    };

}
#endif // !PROCESS_INPUT_H

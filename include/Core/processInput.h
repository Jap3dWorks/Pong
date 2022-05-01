#ifndef PROCESS_INPUT_H
#define PROCESS_INPUT_H

#include "Movements.h"
#include "Actor.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <map>
#include <utility>

namespace Pong {
    /*
    Functor class useful to manage the inputs of an Actor object.
    */
    class ProcessInput
    {
    private:
        Actor*  _move_element = nullptr;
        std::map<int, Movements> _glfw_keys;
        GLFWwindow* _window = nullptr;

    public:
        /*
        Constructor receives a pointer of template class, also a map with the relations
        (Key, Movements::_anyMove_)
        @param move_element: object pointer, this object should override process_keyboard method.
        @param glfw_keys(std::map): relations between keys and movements type.*/
        ProcessInput(Actor* move_element, std::map<int, Movements> glfw_keys,
            GLFWwindow* window) :_glfw_keys(std::move(glfw_keys)), _window(window)
        {
            // typeid
            _move_element = move_element;
        }


        void operator()(double deltaTime) const
        {
            for (auto const& it: _glfw_keys)
            {
                if (glfwGetKey(_window, it.first) == GLFW_PRESS)
                    _move_element->process_keyboard(it.second, deltaTime);
            }
        }
    };
}
#endif // !PROCESS_INPUT_H

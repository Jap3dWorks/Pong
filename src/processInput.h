#include <GLFW/glfw3.h>
#include "pongTypes.h"
#include "Actor.h"
#include "camera.h"

#include <iostream>
#include <map>

#ifndef PROCESS_INPUT
#define PROCESS_INPUT

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
		@param move_element: object pointer, this object should override ProcessKeyboard method.
		@param glfw_keys(std::map): relations between keys and movements type.*/
		ProcessInput(Actor* move_element, std::map<int, Movements> glfw_keys,
			GLFWwindow* window) :_glfw_keys(glfw_keys), _window(window)
		{
			// typeid
			_move_element = move_element;
		}


		void operator()(float deltaTime) const
		{
			for (auto const& it: _glfw_keys)
			{
				if (glfwGetKey(_window, it.first) == GLFW_PRESS)
					_move_element->ProcessKeyboard(it.second, deltaTime);
			}
		}
	};
}
#endif // !PROCESS_INPUT

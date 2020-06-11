// include ogl math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// shader class

#ifndef PONG_TYPES
#define PONG_TYPES

// TODO declare all types here

namespace Pong {
	class Collider;
	class Shape;
	class Actor;
	class APlayer;

	enum class Movements
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

}
#endif // !PONG_TYPES
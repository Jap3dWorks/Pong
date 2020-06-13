#ifndef UTILS
#define UTILS

#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Scene.h"
#include "Actor.h"

// utility to load textures
unsigned int loadTexture(char const *path, const bool &gammaCorrection);


void cout_matrix(const glm::mat4 &matrix);


template<typename T>
void cout_buffer(const std::vector<T> v, int col)
{
	for (int i = 0; i < v.size() / col; i++) 
	{

		for (int j = 0; j < col; j++)
		{
			std::cout << v[(i*col) + j] << " | " << std::flush;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

}

void cout_vector(glm::vec3 &vector);


namespace Pong {
	void draw_point(glm::vec3 point);


}
#endif // !UTILS


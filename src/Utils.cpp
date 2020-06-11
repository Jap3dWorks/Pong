#include "Utils.h"

// utility to load textures
unsigned int loadTexture(char const *path, const bool &gammaCorrection)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;
		if (nrComponents == 1)
			dataFormat = internalFormat = GL_RED;
		else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			internalFormat,
			width,
			height,
			0,
			dataFormat,
			GL_UNSIGNED_BYTE,
			data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


void cout_matrix(const glm::mat4 &matrix)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << matrix[i][j] << " | ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
	

void cout_vector(glm::vec3 &vector)
{
	std::cout << vector.x << ", " << vector.y << ", " << vector.z << std::endl;
}

namespace Pong {
	void draw_point(glm::vec3 point)
	{
		Scene* scene = Scene::getInstance();

		// point actor name
		std::string pnt_nme = "point_";
		int number = 0;
		while (true)
		{
			std::string try_nme = pnt_nme + std::to_string(number);
			//std::cout << "try name-> " << try_nme << std::endl;
			if (scene->actor_map.find(try_nme) == scene->actor_map.end())
			{
				pnt_nme = try_nme;
				break;
			}
			else
			{
				number++;
			}
		}

		// get shader
		Shader* point_shd = scene->create_shader("point_shd", "unlit_v.glsl", "unlit_f.glsl");

		Material* point_mat = scene->createMaterial("point_mat", point_shd,
			std::vector<Pong::Texture*>());

		// point actor
		AKinetic* a_point = scene->createActor<AKinetic>(pnt_nme);
		IcosphereShape* s_point = scene->createShape<IcosphereShape>("point_shp");
		a_point->setShape(s_point);
		a_point->setTransform(glm::translate(glm::mat4(1), point));
		a_point->setMaterial(point_mat);

		s_point->setRadius(0.05f);

		std::cout << "Point created-> " << pnt_nme << " at-> " << point.x << ", " << point.y << 
			", " << point.z  << std::endl;
	}
}
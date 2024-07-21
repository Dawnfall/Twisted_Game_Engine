#pragma once

#include <glad/glad.h>
#include <glm.hpp>

#include <vector>
#include<string>

namespace Dawn
{
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Texture {
		unsigned int id;
		std::string type;
	};

	class Mesh
	{
	public:
		// mesh data
		std::vector<Vertex>       vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture>      textures;

		unsigned int VAO = -1;
		unsigned int VBO = -1;
		unsigned int EBO = -1;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices/*, std::vector<Texture> textures */)
		{
			this->vertices = vertices;
			this->indices = indices;
			//this->textures = textures;		
		}
	};

}
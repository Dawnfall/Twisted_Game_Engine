#pragma once

#include <glad/glad.h>
#include <vector>

namespace Dawn
{
	struct MeshData
	{
		std::vector<float> vertices;
		std::vector<int> indices;

		unsigned int VBO;
		unsigned int IBO;
	};

	class RenderContext
	{
	public:
		void Init()
		{
			glViewport(0, 0, 800, 600); //bot left, width,height
		}
		
		void Render(MeshData& meshData)
		{
			glGenBuffers(1, &meshData.VBO);
			glBindBuffer(GL_ARRAY_BUFFER, meshData.VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(meshData.vertices), &meshData.vertices, GL_STATIC_DRAW);
		}

	private:
	};


}

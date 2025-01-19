#pragma once
#include "AppCore.h"
#include "Data/MeshData.h"

namespace Twisted
{
	class Mesh
	{
	public:
		Mesh(const MeshData& meshData);
		~Mesh();

		void Bind()const;
		void UnBind()const;

		void Render()const;

		size_t IndicesSize = -1;
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;
	};

}
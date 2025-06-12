#pragma once
#include "AppCore.h"

#include <filesystem>
#include <Twisted/Data/Vertex.h>

namespace Twisted::Render
{
	struct TWISTED_API MeshData
	{
		std::filesystem::path Name;
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
	};

	class TWISTED_API Mesh
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
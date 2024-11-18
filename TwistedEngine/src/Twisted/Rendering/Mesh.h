#pragma once
#include "twistedpch.h"
#include "Assets/MeshData.h"
#include "AppCore.h"
namespace Twisted
{
	class TWISTED_API Mesh
	{
	public:
		Mesh(std::shared_ptr<MeshData> meshData,unsigned int vao,unsigned int vbo,unsigned int ebo) :
			Data(meshData),
			VAO(vao),
			VBO(vbo),
			EBO(ebo)
		{}

		std::shared_ptr<MeshData> Data;
		unsigned int VAO = -1;
		unsigned int VBO = -1;
		unsigned int EBO = -1;
	};

}
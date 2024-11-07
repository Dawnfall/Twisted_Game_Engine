#pragma once
#include "twistedpch.h"
#include "Twisted/Resources/MeshData.h"

namespace Twisted
{
	class Mesh
	{
	public:
		Mesh(MeshData meshData) :
			Data(meshData)
		{}

		MeshData Data;
		unsigned int VAO = -1;
		unsigned int VBO = -1;
		unsigned int EBO = -1;
	};

}
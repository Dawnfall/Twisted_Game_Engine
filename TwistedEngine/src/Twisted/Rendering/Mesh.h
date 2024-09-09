#pragma once
#include "pch.h"
#include "Twisted/Resources/MeshData.h"

namespace Twisted
{
	class Mesh
	{
	public:
		std::shared_ptr<MeshData> Data;

		unsigned int VAO = -1;
		unsigned int VBO = -1;
		unsigned int EBO = -1;
	};

}
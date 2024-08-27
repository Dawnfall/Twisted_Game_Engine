#pragma once

#include "Twisted/Resources/MeshData.h"
#include <memory>

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
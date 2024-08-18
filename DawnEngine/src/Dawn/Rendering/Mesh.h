#pragma once

#include "Dawn/Resources/MeshData.h"
#include <memory>

namespace Dawn
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
#pragma once

#include "Resource.h"
#include "Collections/Geometry.h"

#include <vector>

namespace Dawn
{
	struct MeshData : Resource
	{
		// mesh data
		std::vector<Vertex>       vertices;
		std::vector<unsigned int> indices;
	};
}
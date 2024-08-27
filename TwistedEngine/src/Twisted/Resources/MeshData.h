#pragma once

#include "Resource.h"
#include "Collections/Geometry.h"

#include <vector>

namespace Twisted
{
	struct MeshData : Resource
	{
		// mesh data
		std::vector<Vertex>       vertices;
		std::vector<unsigned int> indices;
	};
}
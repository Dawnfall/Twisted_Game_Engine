#pragma once

#include "pch.h"
#include "Resource.h"
#include "Collections/Geometry.h"

namespace Twisted
{
	struct MeshData : Resource
	{
		// mesh data
		std::vector<Vertex>       vertices;
		std::vector<unsigned int> indices;
	};
}
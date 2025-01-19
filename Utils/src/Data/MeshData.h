#pragma once

#include "AppCore.h"
#include "Utils/GlmUtils.h"
#include "Data/Vertex.h"

#include <vector>

namespace Twisted
{
	struct MeshData
	{
		std::vector<Vertex>       Vertices;
		std::vector<unsigned int> Indices;
	};
}
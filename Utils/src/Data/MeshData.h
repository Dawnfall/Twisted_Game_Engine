#pragma once

#include "AppCore.h"
#include "Utils/GlmUtils.h"
#include "Data/Vertex.h"

#include <vector>
#include <filesystem>

namespace Twisted
{
	struct MeshData
	{
		std::filesystem::path Name;
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
	};
}
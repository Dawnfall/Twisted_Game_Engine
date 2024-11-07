#pragma once

#include "twistedpch.h"
#include "Collections/Geometry.h"

namespace Twisted
{
	struct MeshData
	{
		// mesh data
		std::string				  Name;
		std::vector<Vertex>       Vertices;
		std::vector<unsigned int> Indices;
	};
}
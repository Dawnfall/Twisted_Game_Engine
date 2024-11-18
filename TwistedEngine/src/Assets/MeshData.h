#pragma once

#include "twistedpch.h"
#include "Collections/Geometry.h"
#include "Asset.h"
#include "AppCore.h"

namespace Twisted
{
	struct TWISTED_API MeshData:public Asset
	{
		std::vector<Vertex>       Vertices;
		std::vector<unsigned int> Indices;
	};
}
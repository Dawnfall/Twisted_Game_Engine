#pragma once

#include "Collections/Geometry.h"
#include <vector>

using Dawn::Vertex;

namespace Collections
{
	extern const std::vector<Vertex> triangleVertices;
	extern const std::vector<unsigned int> triangleIndices;

	extern const std::vector<Vertex> quadVertices;
	extern const std::vector<unsigned int> quadIndices;

	extern const std::vector<Vertex> cubeVertices;
	extern const std::vector<unsigned int> cubeIndices;
}
#pragma once

#include <vector>

namespace Dawn::Collections
{
	static class ModelCollections
	{
	public:
		static const std::vector<float> testTriangleVertices;
		static const std::vector<int> testTriangleIndices;

		static const std::vector<float> triangleVertices;
		static const std::vector<int> triangleIndices;

		static const std::vector<float> cubeVertices; 
		static const std::vector<int> cubeIndices;
	};
}

#include "ModelCollections.h"

namespace Dawn
{
	const std::vector<float> triangleVertices{
		-0.5f,-0.43301f,1.0f,
		0.5f,-0.43301f,1.0f,
		0.0f,0.43301f,1.0f
	};
	const std::vector<int> triangleIndices{
		0,1,2
	};

	const std::vector<float> ModelCollections::cubeVertices{
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f
	};

	const std::vector<int> ModelCollections::cubeIndices{
		0,1,3,
		1,2,3,
		0,1,4,
		1,5,4,
		1,2,5,
		2,6,5,
		2,3,6,
		3,7,6,
		3,0,7,
		0,4,7,
		4,7,5,
		7,6,5
	};
}
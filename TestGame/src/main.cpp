#include "Dawn.h"
#include "../TestRuntime.h"

//
//#include <string>
//#include <vector>
//#include <Dawn/Mesh.h>



int main()
{
	//Dawn::Vertex vertex;
	//vertex.Position = glm::vec3(0.2f, 0.4f, 0.6f);
	//vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	//vertex.TexCoords = glm::vec2(1.0f, 0.0f);

	//Dawn::Mesh* testMesh = new Dawn::Mesh(triangleVertices,triangleIndices);

	TestRuntime runtime;
	Dawn::Application* app = new Dawn::Application();
	app->Start(runtime);

	//delete testShader;
	delete app;

	return 0;
}
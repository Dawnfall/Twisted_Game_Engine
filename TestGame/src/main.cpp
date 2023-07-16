#include "Dawn.h"

#include <string>
//#include <Dawn/Mesh.h>

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

const std::string vertexShaderCode=
R"(
#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
}
)";

const std::string fragmentShaderCode =
R"(
#version 330 core
out vec4 FragColor;
  
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

void main()
{
    FragColor = vertexColor;
} 
)";

int main()
{
	//Dawn::Vertex vertex;
	//vertex.Position = glm::vec3(0.2f, 0.4f, 0.6f);
	//vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	//vertex.TexCoords = glm::vec2(1.0f, 0.0f);


	Dawn::Application* app = new Dawn::Application();
	app->Init();
	app->Run();
	delete app;

	return 0;
}
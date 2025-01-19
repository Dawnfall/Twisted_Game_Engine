#pragma once
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/FrameBuffer.h"

#include "Data/MeshData.h"
#include "Data/TextureData.h"
#include "Data/ShaderData.h"
#include "Utils/Color.h"
#include "AppCore.h"

#include <glad/glad.h>

namespace Twisted::RenderAPI
{
	bool InitGLFW();
	bool InitOpenGL(GLADloadproc loadproc);
	void TerminateGLFW();

	void glfwErrorCallback(int code, const char* description);
	void openGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	//************
	// Shader

	GLuint CreateShaderProgram(const std::string& vertexShaderCode, const std::string& fragShaderCode);
	GLuint CompileProgram(GLuint vertexID, GLuint fragmentID);
	GLuint CompileShaderCode(GLenum shaderType, const char* shaderName, const char* shaderCode);
};
// detect uniforms different way

/*void Detect()
{
	GLint numActiveAttribs = 0;
	GLint numActiveUniforms = 0;
	glGetProgramInterfaceiv(ID, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);
	glGetProgramInterfaceiv(ID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

	std::vector<GLchar> nameData(256);
	std::vector<GLenum> properties;
	properties.push_back(GL_NAME_LENGTH);
	properties.push_back(GL_TYPE);
	properties.push_back(GL_ARRAY_SIZE);

	std::vector<GLint> values(properties.size());
	for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
	{
		glGetProgramResourceiv(ID, GL_PROGRAM_INPUT, attrib, properties.size(), &properties[0], values.size(), NULL, &values[0]);

		nameData.resize(values[0]);
		glGetProgramResourceName(ID, GL_PROGRAM_INPUT, attrib, nameData.size(), NULL, &nameData[0]);
		std::string name((char*)&nameData[0], nameData.size() - 1);
	}
}*/
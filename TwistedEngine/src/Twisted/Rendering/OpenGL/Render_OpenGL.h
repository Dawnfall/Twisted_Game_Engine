#pragma once

#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/Shader.h"
#include "Twisted/Resources/ShaderData.h"
#include "Twisted/Rendering/Texture.h"
#include "Twisted/Game/Components/CRenderer.h"
#include "Twisted/Game/Components/CTransform.h"

#include "Collections/Color.h"
#include <memory>
#include <string>

namespace Twisted
{
	class Render_OpenGL
	{
	public:
		static bool InitGLFW();
		static bool InitOpenGL(GLADloadproc loadproc);
		static void Terminate();

		static void Render(const CRenderer& renderer);
		static void UploadMesh(std::shared_ptr<Mesh>& mesh);

		static void ClearWindow(Collections::Color color);
		static void SetViewPort(float width, float height);
		static void SetVsync(int deltaFrames);

		//	//************
		//	// Shader

		static std::shared_ptr<Shader> CreateShader(const std::shared_ptr<ShaderData>& shaderData);
		static void UnCompileShader(Shader& shader);

		static void SetUniforms(const std::shared_ptr<Material>& material);

		// Utility uniform functions
		static void setBool(GLint locationID, bool value);
		static void setInt(GLint locationID, int value);
		static void setFloat(GLint locationID, float value);
		static void setFloat2(GLint locationID, const Vec2f& value);
		static void setFloat3(GLint locationID, const Vec3f& value);
		static void setFloat4(GLint locationID, const Vec4f& value);
		static void setMat4(GLint locationID, const Mat4x4f& value);

		//*****************************
		// Texture

		static std::shared_ptr<Texture> LoadTexture(const std::string& name, int width, int height, unsigned char* data)
		{
			GLuint id;

			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			std::shared_ptr<Texture> newTex = std::make_shared<Texture>();
			newTex->ID = id;
			newTex->Name = name;

			return newTex;
		}

	private:
		static GLuint CompileShader(GLenum shaderType, const char* shaderName, const char* shaderCode);
		static GLuint CompileProgram(GLuint vertexID, GLuint fragmentID);
		static std::vector<ShaderUniformVar> DetectUniformVars(GLuint programID);

		//	static void BindTexture(int id);
		static void glfwErrorCallback(int code, const char* description);
		static void openGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);



	};
}

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
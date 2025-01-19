#include "renderingpch.h"

#include "AppCore.h"
#include "Render_OpenGL.h"

namespace Twisted::RenderAPI
{
	bool InitGLFW()
	{
		if (!glfwInit())
		{
			TWISTED_ERROR("GLFW init failure; RenderCore Init failure!");
			return false;
		}
		glfwSetErrorCallback(glfwErrorCallback);

		TWISTED_INFO("GLFW init success");
		TWISTED_INFO("RenderCore Init success!");

		return true;
	}

	bool InitOpenGL(GLADloadproc loadproc)
	{
		if (!gladLoadGLLoader(loadproc)) {
			TWISTED_ERROR("CreateNewWindow() failure! GLAD init failure");
			return false;
		}

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glDebugMessageCallback(openGLErrorCallback, 0);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		TWISTED_INFO("OpenGL init success");
	}

	void TerminateGLFW()
	{
		glfwTerminate();
	}

	////**************
	//// Other

	GLuint CreateShaderProgram(const std::string& vertexShaderCode, const std::string& fragShaderCode)
	{
		GLuint vertexID = 0;
		GLuint fragmentID = 0;

		if (vertexShaderCode != "")
			vertexID = RenderAPI::CompileShaderCode(GL_VERTEX_SHADER, "Vertex", vertexShaderCode.c_str());
		if (fragShaderCode != "")
			fragmentID = RenderAPI::CompileShaderCode(GL_FRAGMENT_SHADER, "Fragment", fragShaderCode.c_str());
		GLuint programID = RenderAPI::CompileProgram(vertexID, fragmentID);

		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);

		if (programID < 1)
		{
			TWISTED_WARN("Shader compile failure; shader: ");
			return 0;
		}
		return programID;
	}
	GLuint CompileShaderCode(GLenum shaderType, const char* shaderName, const char* shaderCode)
	{
		GLuint shaderID = glCreateShader(shaderType);

		if (shaderID == 0)
		{
			TWISTED_ERROR("Error creating shader");
			return 0;
		}

		glShaderSource(shaderID, 1, &shaderCode, NULL);
		glCompileShader(shaderID);

		int success;
		char infoLog[512];
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (success != GL_TRUE)
		{
			glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
			TWISTED_ERROR(std::string("Error: Failed to compile ") + shaderName + " shader! " + infoLog);
			return 0;
		}
		return shaderID;
	}
	GLuint CompileProgram(GLuint vertexID, GLuint fragmentID)
	{
		GLuint shaderProgramID = glCreateProgram();
		glAttachShader(shaderProgramID, vertexID);
		glAttachShader(shaderProgramID, fragmentID);
		glLinkProgram(shaderProgramID);

		int success;
		char infoLog[512];

		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
		if (success != GL_TRUE)
		{
			glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
			TWISTED_ERROR(std::string("Error: Failed to compile shader program!") + infoLog);
			return 0;
		}
		return shaderProgramID;
	}

	void glfwErrorCallback(int code, const char* description)
	{
		TWISTED_ERROR(description);
	}

	void openGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::cout << "---------------" << std::endl;
		std::cout << "Debug message (" << id << "): " << message << std::endl;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
		} std::cout << std::endl;

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
		} std::cout << std::endl;

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
		} std::cout << std::endl;
		std::cout << std::endl;

		//fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		//	(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		//	type, severity, message);
	}

}
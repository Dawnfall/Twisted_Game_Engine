#include "Twisted/Rendering/RenderingAPI.h"

#include <glad/glad.h>
#include <iostream>
#include <format>
#include "Debug/Logger.h"

namespace Twisted
{
	void openGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::string msg = std::format("Debug message({}) : {}\n", id, message);

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             msg += "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   msg += "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: msg += "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     msg += "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     msg += "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           msg += "Source: Other"; break;
		} msg += "\n";

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               msg += "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: msg += "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  msg += "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         msg += "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         msg += "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              msg += "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          msg += "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           msg += "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               msg += "Type: Other"; break;
		} msg += "\n";

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         msg += "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       msg += "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          msg += "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: msg += "Severity: notification"; break;
		} msg += "\n";

		TWISTED_WARN(msg);

		//fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		//	(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		//	type, severity, message);
	}

	bool InitRenderer(void* context)
	{
		if (!gladLoadGLLoader(static_cast<GLADloadproc>(context))) {
			TWISTED_ERROR("GLAD init failure");
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
		return true;
	}
}
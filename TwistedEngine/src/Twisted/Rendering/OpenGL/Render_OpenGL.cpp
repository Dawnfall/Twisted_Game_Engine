#include "pch.h"
#include "Render_OpenGL.h"
#include "Debug/Logger.h"

namespace Twisted
{
	bool Render_OpenGL::InitGLFW()
	{
		if (!glfwInit())
		{
			TWISTED_ERROR("GLFW init failure; RenderCore Init failure!");
			return false;
		}
		glfwSetErrorCallback(Render_OpenGL::glfwErrorCallback);

		TWISTED_INFO("GLFW init success");
		TWISTED_INFO("RenderCore Init success!");

		return true;
	}

	bool Render_OpenGL::InitOpenGL(GLADloadproc loadproc)
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
		glDebugMessageCallback(Render_OpenGL::openGLErrorCallback, 0);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		TWISTED_INFO("OpenGL init success");
	}

	void Render_OpenGL::Terminate()
	{
		glfwTerminate();
	}

	std::shared_ptr<Mesh> Render_OpenGL::CreateMesh(MeshData meshData)
	{
		auto mesh = std::make_shared<Mesh>(meshData);

		glGenVertexArrays(1, &mesh->VAO);
		glGenBuffers(1, &mesh->VBO);
		glGenBuffers(1, &mesh->EBO);

		glBindVertexArray(mesh->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->Data.Vertices.size() * sizeof(Vertex), &mesh->Data.Vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->Data.Indices.size() * sizeof(unsigned int), &mesh->Data.Indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

		glBindVertexArray(0);

		return mesh;
	}

	////***************
	//// Shader

	void Render_OpenGL::UnCompileShader(Shader& shader)
	{
		glDeleteShader(shader.ProgramID);
		shader.ProgramID = -1;
	}

	std::shared_ptr<Shader> Render_OpenGL::CreateShader(ShaderData shaderData)
	{
		GLuint vertexID = 0;
		GLuint fragmentID = 0;

		if (shaderData.VertShaderCode != "")
			vertexID = Render_OpenGL::CompileShader(GL_VERTEX_SHADER, "Vertex", shaderData.VertShaderCode.c_str());
		if (shaderData.FragShaderCode != "")
			fragmentID = Render_OpenGL::CompileShader(GL_FRAGMENT_SHADER, "Fragment", shaderData.FragShaderCode.c_str());
		GLuint programID = Render_OpenGL::CompileProgram(vertexID, fragmentID);

		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);

		if (programID < 1)
		{
			TWISTED_WARN("Shader compile failure; shader: " + shaderData.Name);
			return nullptr;
		}

		glUseProgram(programID);
		std::vector<ShaderUniformVar> uniforms = DetectUniformVars(programID);

		TWISTED_INFO("Shader compile success; shader: " + shaderData.Name);
		return std::make_shared<Shader>(shaderData, programID, uniforms);
	}

	GLuint Render_OpenGL::CompileShader(GLenum shaderType, const char* shaderName, const char* shaderCode)
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

	GLuint Render_OpenGL::CompileProgram(GLuint vertexID, GLuint fragmentID)
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

	std::vector<ShaderUniformVar> Render_OpenGL::DetectUniformVars(GLuint programID)
	{
		std::vector<ShaderUniformVar> uniforms;
		std::vector<ShaderTextureVar> textures;

		GLint count = 1;
		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 16; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length

		glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);

		printf("Active Uniforms: %d\n", count);

		GLint textureUnit = 0;
		for (int i = 0; i < count; i++)
		{
			glGetActiveUniform(programID, (GLuint)i, bufSize, &length, &size, &type, name);
			printf("Uniform #%d Type: %u Name: %s\n", i, type, name);

			GLint uniformID = glGetUniformLocation(programID, name);

			if (type == GL_SAMPLER_2D)
			{
				glUniform1i(uniformID, textureUnit);
				textures.emplace_back(name, type, uniformID, textureUnit++);
			}
			else
				uniforms.emplace_back(name, type, uniformID);
		}
		return uniforms;
	}

	void Render_OpenGL::SetUniforms(const std::shared_ptr<Material>& material)
	{
		for (auto& var : material->m_bools)
			setBool(var.second.ID, var.second.value);
		for (auto& var : material->m_ints)
			setInt(var.second.ID, var.second.value);
		for (auto& var : material->m_floats)
			setFloat(var.second.ID, var.second.value);
		for (auto& var : material->m_vec2s)
			setFloat2(var.second.ID, var.second.value);
		for (auto& var : material->m_vec3s)
			setFloat3(var.second.ID, var.second.value);
		for (auto& var : material->m_vec4s)
			setFloat4(var.second.ID, var.second.value);
		for (auto& var : material->m_mats)
			setMat4(var.second.ID, var.second.value);
		for (auto& var : material->m_textures)
		{
			glActiveTexture(GL_TEXTURE0 + var.first);
			glBindTexture(GL_TEXTURE_2D, var.second->ID);
		}
	}

	void Render_OpenGL::setBool(GLint locationID, bool value)
	{
		glUniform1i(locationID, static_cast<int>(value));
	}
	void Render_OpenGL::setInt(GLint locationID, int value)
	{
		glUniform1i(locationID, value);
	}
	void Render_OpenGL::setFloat(GLint locationID, float value)
	{
		glUniform1f(locationID, value);
	}
	void Render_OpenGL::setFloat2(GLint locationID, const Vec2f& value)
	{
		glUniform2f(locationID, value[0], value[1]);
	}
	void Render_OpenGL::setFloat3(GLint locationID, const Vec3f& value)
	{
		glUniform3f(locationID, value[0], value[1], value[2]);
	}
	void Render_OpenGL::setFloat4(GLint locationID, const Vec4f& value)
	{
		glUniform4f(locationID, value[0], value[1], value[2], value[3]);
	}
	void Render_OpenGL::setMat4(GLint locationID, const Mat4x4f& value)
	{
		glUniformMatrix4fv(locationID, 1, GL_FALSE, &value[0][0]);
	}

	////**************
	//// Other

	void Render_OpenGL::Render(const CRenderer& renderer)
	{
		glUseProgram(renderer.Material->GetShader()->ProgramID);
		SetUniforms(renderer.Material);
		glBindVertexArray(renderer.Mesh->VAO);
		glDrawElements(GL_TRIANGLES, (GLsizei)renderer.Mesh->Data.Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	//void Render_OpenGL::BindTexture(int id)
	//{
	//	//TODO:.....
	//}

	void Render_OpenGL::ClearWindow(Colors::Color color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void Render_OpenGL::SetViewPort(float width, float height)
	{
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	}
	void Render_OpenGL::SetVsync(int deltaFrames)
	{
		glfwSwapInterval(deltaFrames);
	}

	void Render_OpenGL::glfwErrorCallback(int code, const char* description)
	{
		TWISTED_ERROR(description);
	}

	void Render_OpenGL::openGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
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
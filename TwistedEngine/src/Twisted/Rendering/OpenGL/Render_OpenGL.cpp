#include "twistedpch.h"
#include "Render_OpenGL.h"
#include "Debug/Logger.h"
#include "Shader_OpenGL.h"

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

	void Terminate()
	{
		glfwTerminate();
	}

	std::shared_ptr<Mesh> CreateMesh(MeshData meshData)
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

	////**************
	//// Other

	void Render(const CRenderer& renderer)
	{
		glUseProgram(renderer.Material->GetShader()->ProgramID);
		ShaderAPI::SetUniforms(renderer.Material);
		glBindVertexArray(renderer.Mesh->VAO);
		glDrawElements(GL_TRIANGLES, (GLsizei)renderer.Mesh->Data.Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	//void Render_OpenGL::BindTexture(int id)
	//{
	//	//TODO:.....
	//}

	void ClearBuffers(bool doClearColor, Colors::Color clearColor, bool doClearDepth, bool doClearStencil)
	{
		int bufferBits = 0;
		if (doClearColor)
		{
			bufferBits |= GL_COLOR_BUFFER_BIT;
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		}
		if (glClearStencil)
			bufferBits |= GL_STENCIL_BUFFER_BIT;
		if (doClearDepth)
			bufferBits |= GL_DEPTH_BUFFER_BIT;

		glClear(bufferBits);
	}

	void SetEnableDepthTest(bool doEnable)
	{
		glEnable(GL_STENCIL_TEST);
	}

	void SetEnableDepthWrite(bool doEnable)
	{
		if (doEnable)
			glDepthMask(doEnable);
	}

	void SetEnableStencilTest(bool doEnable)
	{
		if (doEnable)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
	}

	void ClearWindow(Colors::Color color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void SetViewPort(float width, float height)
	{
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	}
	void SetVsync(int deltaFrames)
	{
		glfwSwapInterval(deltaFrames);
	}

	void glfwErrorCallback(int code, const char* description)
	{
		TWISTED_ERROR(description);
	}

	std::shared_ptr<Texture> LoadTexture(const std::string& name, int width, int height, unsigned char* data)
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
		newTex->TextureID = id;
		newTex->Name = name;
		//newTex->Type = TextureType::TEXTURE2D;

		return newTex;
	}
	
	
	//void BindTexture(std::shared_ptr<Texture> texture)
	//{
	//	GLenum texType = GL_TEXTURE_2D;
	//	switch (texture->Type)
	//	{
	//	case TextureType::TEXTURE2D:
	//		texType = GL_TEXTURE_2D;
	//		break;
	//	case TextureType::CUBEMAP:
	//		texType = GL_TEXTURE_CUBE_MAP;
	//		break;
	//	default:
	//		break;
	//	}
	//	glBindTexture(texType, texture->TextureID);
	//}

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
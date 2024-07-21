#include "RenderCore.h"
#include "Collections/Color.h"
#include <glad/glad.h>
#include "unfinised/Mesh.h"

namespace Dawn
{
	bool RenderCore::Init()
	{
		if (!glfwInit())
		{
			DAWN_ERROR("GLFW init failure; RenderCore Init failure!");
			return false;
		}
		glfwSetErrorCallback(RenderCore::ErrorCallback);
		DAWN_INFO("GLFW init success");
		DAWN_INFO("RenderCore Init success!");

		return true;
	}

	void RenderCore::BindMesh(float* vertices, unsigned int verticesSize, unsigned int* indices, unsigned indicesSize,
		unsigned int& vao, unsigned int& vbo, unsigned int& ebo)
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(Vertex), vertices/*&vertices[0]*/, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int),
			indices, GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

	void RenderCore::UnBindMesh()
	{
		//TODO:...??
	}

	void RenderCore::BindTexture(int id)
	{

	}

	void RenderCore::Render(unsigned int vao, unsigned int indicesSize)
	{
		// draw mesh
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void RenderCore::Terminate()
	{
		glfwTerminate();
	}

	void RenderCore::ErrorCallback(int code, const char* description)
	{
		DAWN_ERROR(description);
	}

	void RenderCore::ClearWindow(Collections::Color color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void RenderCore::ClearWindows(const std::vector<std::shared_ptr<Window>>& windows, Collections::Color color)
	{
		for (auto& window : windows)
		{
			glfwMakeContextCurrent(window->Pointer);
			RenderCore::ClearWindow(color);
		}
	}


	void RenderCore::SetViewPort(float width, float height)
	{
		glViewport(0, 0, width, height);
	}

	void RenderCore::SetVsync(int deltaFrames)
	{
		glfwSwapInterval(deltaFrames);
	}
	
}
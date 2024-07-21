#pragma once

#include <memory>
#include <vector>

#include "Dawn/Window.h"
#include "Debug/Logger.h"
#include "Collections/Color.h"

namespace Dawn
{
	class RenderCore
	{
	public:
		static bool Init();
		static void Terminate();

		static void BindMesh(float* vertices, unsigned int verticesSize, unsigned int* indices, unsigned indicesSize,
			unsigned int& vao, unsigned int& vbo, unsigned int& ebo);
		static void UnBindMesh();

		static void Render(unsigned int vao, unsigned int indicesSize);
		static void ClearWindow(Collections::Color color);
		static void ClearWindows(const std::vector<std::shared_ptr<Window>>& windows, Collections::Color color);
		static void SetViewPort(float width, float height);
		static void SetVsync(int deltaFrames);

	private:
		static void BindTexture(int id);
		static void ErrorCallback(int code, const char* description);
	};
}

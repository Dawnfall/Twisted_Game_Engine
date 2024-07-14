#pragma once

#include <vector>
#include <string>

#include "Shader.h"
#include "Mesh.h"
#include "Dawn/Window.h"
#include "Utils/Color.h"

namespace Dawn
{
    class Window;

	class RenderContext
	{
	public:
        static void Init();
		static void Render(Mesh& mesh, Shader& shader);

		static void Clear(Color color);
		static void SetViewPort(float width,float height);
	private:
	};


}

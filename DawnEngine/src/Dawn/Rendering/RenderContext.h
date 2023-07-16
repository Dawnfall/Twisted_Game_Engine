#pragma once

#include <vector>
#include <string>

#include "Shader.h"
#include "Mesh.h"
#include "Dawn/Window.h"

namespace Dawn
{
    class Window;

	class RenderContext
	{
	public:
        void Init(Window* window);
		
		void Render(Mesh& mesh, Shader& shader);

	private:
	};


}

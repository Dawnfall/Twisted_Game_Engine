#include "twistedpch.h"
#include "ShaderCompilation.h"
#include "Assets/ShaderData.h"
#include "Shader.h"
#include "Utils/FileUtils.h"

#include "RenderingAPI.h"
#include "OpenGL/Shader_OpenGL.h"

namespace Twisted::ShaderCompilation
{
	std::shared_ptr<Shader> LoadShader(std::shared_ptr<ShaderData> shaderData)
	{
		return ShaderAPI::CreateShader(shaderData);
	}
}
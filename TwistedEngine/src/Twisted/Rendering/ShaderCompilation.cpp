#include "twistedpch.h"
#include "ShaderCompilation.h"
#include "Assets/ShaderData.h"
#include "Shader.h"
#include "Utils/FileUtils.h"

#include "RenderingAPI.h"
#include "OpenGL/Shader_OpenGL.h"

namespace Twisted::ShaderCompilation
{
	SRef<Shader> LoadShader(SRef<ShaderData> shaderData)
	{
		return ShaderAPI::CreateShader(shaderData);
	}
}
#pragma once
#include "twistedpch.h"

#include "Shader.h"
#include "AppCore.h"

namespace Twisted::ShaderCompilation
{
	std::shared_ptr<Shader> TWISTED_API LoadShader(std::shared_ptr<ShaderData> shaderData);
}

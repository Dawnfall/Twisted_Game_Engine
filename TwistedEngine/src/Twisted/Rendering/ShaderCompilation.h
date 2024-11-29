#pragma once
#include "twistedpch.h"

#include "Shader.h"
#include "AppCore.h"

namespace Twisted::ShaderCompilation
{
	SRef<Shader> TWISTED_API LoadShader(SRef<ShaderData> shaderData);
}

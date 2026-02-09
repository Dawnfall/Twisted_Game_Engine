#pragma once
#include "AppCore.h"
#include "Twisted/TObject.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Rendering/Data/ShaderData.h"
#include <unordered_map>
#include <string>
#include <vector>

namespace Twisted
{
	//class Shader_OpenGL;
	class Material;

	struct TWISTED_API Shader :public TObject
	{
		Shader(const std::string& name);

		void OnDestroy()override;
		void ApplyUniforms(const Material* material);
		void SetData(const ShaderData& data);

	private:
		//SRef<Shader_OpenGL> m_resource;
	};

}



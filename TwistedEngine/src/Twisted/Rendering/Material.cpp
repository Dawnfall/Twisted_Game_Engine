#include "Twisted/Rendering/Material.h"

#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Twisted/Rendering/Shader.h"
#include <yaml-cpp/node/node.h>
#include <Utils/YamlUtils.h>
#include "Utils/WPtr.h"
#include <variant>
#include <string>
#include <type_traits>

#include "Twisted/Rendering/OpenGL/Shader_OpenGL.h"
#include "Utils/YamlUtils.h"

namespace Twisted
{
	void Material::ApplyUniforms()
	{
		if (!Shader)
			return;

		for (auto& uniform : Shader->Uniforms)
		{
			auto it = Properties.find(uniform.Name);
			if (it == Properties.end())
				continue;

			switch (uniform.Type)
			{
			case ShaderVarType::BOOL:
				Shader_GL::SetVar(uniform.UniformID, Get<bool>(uniform.Name, false));
				break;
			case ShaderVarType::INT:
				Shader_GL::SetVar(uniform.UniformID, Get<int>(uniform.Name, 0));
				break;
			case ShaderVarType::FLOAT:
				Shader_GL::SetVar(uniform.UniformID, Get<float>(uniform.Name, 0.0f));
				break;
			case ShaderVarType::VEC2_F:
				Shader_GL::SetVar(uniform.UniformID, Get<Vec2f>(uniform.Name, Vec3f{ 1.0f,1.0f,1.0f }));
				break;
			case ShaderVarType::VEC3_F:
				Shader_GL::SetVar(uniform.UniformID, Get<Vec3f>(uniform.Name, Vec3f{ 1.0f,1.0f,1.0f }));
				break;
			case ShaderVarType::VEC4_F:
				Shader_GL::SetVar(uniform.UniformID, Get<Vec4f>(uniform.Name, Vec4f{ 1.0f,1.0f,1.0f,1.0f }));
				break;
			case ShaderVarType::MAT4x4_F:
				Shader_GL::SetVar(uniform.UniformID, Get<Mat4x4f>(uniform.Name, Mat4x4f{ 1.0f }));
				break;
			case ShaderVarType::SAMPLER2D:
			{
				auto tex = Get<WPtr<Texture>>(uniform.Name, nullptr);
				if (tex)
					Shader_GL::SetTex(uniform.UniformID, uniform.TextureUnit, tex->TexID);
				else
					Shader_GL::SetTex(uniform.UniformID, uniform.TextureUnit, 0);
				break;
			}
			}
		}
	}
}


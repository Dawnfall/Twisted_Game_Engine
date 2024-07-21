#pragma once

#include "Dawn/Resources/Resource.h"
#include "Dawn/Rendering/Shader.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace Dawn
{
	class ResourceManager
	{
	public:
		template<typename T>
		void AddResource(T& newRes)
		{
			m_resources[newRes.Name] = std::make_shared<T>(newRes);
		}

		template<typename T>
		std::shared_ptr<Resource> GetResource(const std::string& name)
		{
			if (m_resources.contains(name))
				return std::dynamic_pointer_cast<T>(m_resources[name]);
			return nullptr;
		}

		void CompileShaders()
		{
			for (auto& keyVal : m_resources)
			{
				auto shaderData = std::dynamic_pointer_cast<ShaderData>(keyVal.second);
				if (shaderData)
				{
					auto shader = std::make_shared<Shader>(shaderData);
					Shader::CompileShader(*shader);
					m_shaders[shaderData->Name] = shader;
				}
			}
		}

		std::weak_ptr<Shader> GetShader(const std::string& shaderName)
		{
			if (m_shaders.contains(shaderName))
				return m_shaders[shaderName];
			return std::weak_ptr<Shader>();
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
	};
}
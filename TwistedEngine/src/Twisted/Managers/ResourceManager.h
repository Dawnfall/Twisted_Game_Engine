#pragma once

#include "Twisted/Rendering/Texture.h"
#include "Twisted/Rendering/Shader.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"
#include "Debug/Logger.h"

#include "AppCore.h"
#include "Utils/FileUtils.h"

//**************

namespace Twisted
{
	class TWISTED_API ResourceManager
	{
	public:
		void LoadAssets(const std::filesystem::path& assetFolder);

		std::shared_ptr<Material> CreateNewMaterial(const std::string& name);
		const std::shared_ptr<Material> GetMaterial(const std::string matrialName);
		const std::shared_ptr<Shader> GetShader(const std::string& shaderName);
		const std::shared_ptr<Texture> GetTexture(const std::string& textureName);
		const std::shared_ptr<Mesh> GetMesh(const std::string& meshName);

		template<typename T>
		bool AddAsset(const std::string& name, std::shared_ptr<T> asset)
		{
			return m_assets.try_emplace(name, asset).second;
		}

	private:

		std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
		std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshes;

		std::unordered_map<std::string, std::shared_ptr<Asset>> m_assets;

	};
}
#pragma once

#include "AppCore.h"
#include "Assets/Asset.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "AppCore.h"

#include "Utils/FileUtils.h"

//**************

namespace Twisted
{
	class TWISTED_API ResourceManager
	{
	public:
		void LoadAssets(const std::filesystem::path& assetFolder);

		SRef<Material> CreateNewMaterial(const std::string& name);
		const SRef<Material> GetMaterial(const std::string matrialName);
		const SRef<Shader> GetShader(const std::string& shaderName);
		const SRef<Texture> GetTexture(const std::string& textureName);
		const SRef<Mesh> GetMesh(const std::string& meshName);

		template<typename T>
		bool AddAsset(const std::string& name, SRef<T> asset)
		{
			return m_assets.try_emplace(name, asset).second;
		}

	private:

		std::unordered_map<std::string, SRef<Resource>> m_resources;
		std::unordered_map<std::string, SRef<Material>> m_materials;
		std::unordered_map<std::string, SRef<Texture>> m_textures;
		std::unordered_map<std::string, SRef<Shader>> m_shaders;
		std::unordered_map<std::string, SRef<Mesh>> m_meshes;

		std::unordered_map<std::string, SRef<Asset>> m_assets;

	};
}
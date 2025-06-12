#pragma once

#include "AppCore.h"
#include "Twisted/Data/Asset.h"
#include "Twisted/Rendering/Texture.h"
#include "Twisted/Rendering/Shader.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/Model.h"
#include "Twisted/Data/ShaderData.h"
#include "Utils/FileUtils.h"

#include <unordered_map>
#include <string>
#include <memory>
//**************

namespace Twisted
{
	URef<ShaderData> TWISTED_API ImportShaderData(const std::filesystem::path& assetPath);
	URef<TextureData> TWISTED_API ImportTextureData(const std::filesystem::path& assetPath);

	//class TWISTED_API ResourceManager
	//{
	//public:
	//	const SRef<Texture> GetTexture(const std::filesystem::path& path);
	//	const SRef<Shader> GetShader(const std::filesystem::path& path);
	//	SRef<Model> GetModel(const std::filesystem::path& assetPath);

	//	//SRef<Asset> ImportAsset(const std::filesystem::path& assetPath);
	//	void LoadAssets(const std::filesystem::path& assetFolder);

	//	const SRef<Mesh> GetMesh(const std::filesystem::path& path);
	//	SRef<Material> CreateNewMaterial(const std::string& name);
	//	const SRef<Material> GetMaterial(const std::string matrialName);

	//	template<typename T>
	//	bool AddAsset(const std::string& name, SRef<T> asset)
	//	{
	//		return m_assets.try_emplace(name, asset).second;
	//	}

	//private:

	//	std::unordered_map<std::string, SRef<Resource>> m_resources;
	//	std::unordered_map<std::string, SRef<Material>> m_materials;
	//	std::unordered_map<std::string, SRef<Texture>> m_textures;
	//	std::unordered_map<std::string, SRef<Shader>> m_shaders;
	//	std::unordered_map<std::string, SRef<Mesh>> m_meshes;
	//	std::unordered_map<std::string, SRef<Model>> m_models;
	//	std::unordered_map<std::string, SRef<Asset>> m_assets;

	//};
}
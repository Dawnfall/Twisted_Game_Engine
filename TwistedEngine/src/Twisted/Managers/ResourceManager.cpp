#include "twistedpch.h"
#include "ResourceManager.h"
#include "Debug/Logger.h"
#include "Collections/Meshes/MeshCollections.h"
#include "Twisted/Rendering/ShaderCompilation.h"
#include "Utils/FileUtils.h"
#include "Constants.h"
#include "Assets/AssetImporter.h"

namespace Twisted
{
	void ResourceManager::LoadAssets(const std::filesystem::path& assetsFolder)
	{
		auto allAssetFiles = Utils::LoadFiles(std::filesystem::path(assetsFolder), "");
		for (auto& file : allAssetFiles)
		{
			std::shared_ptr<Asset> newAsset = AssetImporter::ImportAsset(file);
			if (newAsset)
				m_assets[file.stem().string()] = newAsset;
		}
	}

	const std::shared_ptr<Material> ResourceManager::GetMaterial(const std::string matrialName)
	{
		if (m_materials.contains(matrialName))
			return m_materials[matrialName];
		return nullptr;
	}

	const std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& shaderName)
	{
		if (m_shaders.contains(shaderName))
			return m_shaders[shaderName];
		return nullptr;
	}

	const std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& textureName)
	{
		if (m_textures.contains(textureName))
			return m_textures[textureName];
		return nullptr;
	}

	const std::shared_ptr<Mesh> ResourceManager::GetMesh(const std::string& meshName)
	{
		if (m_meshes.contains(meshName))
			return m_meshes[meshName];
		return nullptr;
	}

	std::shared_ptr<Material> ResourceManager::CreateNewMaterial(const std::string& name)
	{
		if (m_materials.contains(name))
		{
			//TODO....DAWN_WARN("Cannot create material: " + name + " ; Name already exists");
			return nullptr;
		}
		m_materials[name] = std::make_shared<Material>();
		return m_materials[name];
	}

	//void ResourceManager::LoadDefaultResources()
	//{
	//	auto defaultShader = ShaderCompilation::LoadShader(BUILT_IN_DEFAULT_SHADER_PATH, BUILT_IN_DEFAULT_SHADER_NAME);
	//	if (defaultShader)
	//		m_shaders[defaultShader->Name] = defaultShader;

	//	auto simpleShader = ShaderCompilation::LoadShader(BUILT_IN_SIMPLE_SHADER_PATH, BUILT_IN_SIMPLE_SHADER_NAME);
	//	if (simpleShader)
	//		m_shaders[simpleShader->Name] = simpleShader;

	//	LoadMesh(Collections::triangleMeshName, Collections::triangleVertices, Collections::triangleIndices);
	//	LoadMesh(Collections::quadMeshName, Collections::quadVertices, Collections::quadIndices);
	//	LoadMesh(Collections::cubeMeshName, Collections::cubeVertices, Collections::cubeIndices);
	//}
}
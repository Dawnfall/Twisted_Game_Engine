#include "twistedpch.h"
#include "AssetImporter.h"
#include "Utils/FileUtils.h"
#include "Assets/MetaFile.h"
#include "Twisted/Rendering/Texture.h"
#include "Assets/TextureData.h"
#include "Constants.h"

namespace Twisted::AssetImporter
{
	//void AssetImporter::Update(const std::filesystem::path& projectPath)
	//{
	//	auto allMetas = Utils::LoadFiles(projectPath / "Meta", ".meta");
	//	auto allResources = Utils::LoadFiles(projectPath / "Internal", ".internal");
	//	auto allAssets = Utils::LoadFiles(projectPath / "Assets", "");
	//	std::unordered_set<std::string> metaUuids;
	//	std::unordered_set<std::string> metaFileNames;
	//	std::unordered_set<std::string> resourcesUuids;
	//	std::unordered_set<std::string> assetNames;
	//	std::vector<MetaFile> metaFiles;
	//	for (auto& meta : allMetas)
	//	{
	//		MetaFile metaFile;
	//		metaFile.Read(meta.string());
	//		metaFiles.emplace_back(metaFile);
	//		metaUuids.insert(metaFile.Uuid);
	//		metaFileNames.insert(metaFile.AssetName);
	//	}
	//	for (auto& resourcePath : allResources)
	//	{
	//		resourcesUuids.insert(resourcePath.stem().string());
	//	}
	//	for (auto& assetPath : allResources)
	//	{
	//		/*		assetNames.insert(assetPath.filename().string());
	//				if (metaFileNames.contains(assetPath.filename().string()))
	//				{
	//					if (resourcesUuids.contains()
	//				}*/
	//	}
	//}

	std::shared_ptr<Asset> ImportAsset(const std::filesystem::path& assetPath)
	{
		std::string ext = assetPath.extension().string();

		if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".tga" || ext == "bmp" || ext == ".psd"); //TODO....
		{
			return ImportTextureData(assetPath);
		}
		if (ext == ".shader")
		{
			return ImportShaderData(assetPath);
		}
		if (ext == ".obj")
		{
			return ImportMeshData(assetPath);
		}

		TWISTED_WARN("Asset not supported: " + assetPath.filename().string());
		return nullptr;
	}

	std::shared_ptr<TextureData> ImportTextureData(const std::filesystem::path& assetPath)
	{
		std::shared_ptr<TextureData> newTex = std::make_shared<TextureData>();
		newTex->data = stbi_load(assetPath.string().c_str(), &newTex->Width, &newTex->Height, &newTex->NrChannels, 0);
		if (newTex->data)
		{
			//write

			//RenderAPI::LoadTexture(name, width, height, data);
			//m_textures[name] = newTex;
		}

		return newTex;
	}

	std::shared_ptr<ShaderData> ImportShaderData(const std::filesystem::path& assetPath)
	{
		std::string shaderText = Utils::ReadFileContent(assetPath);
		std::vector<std::string> shaderCodes = Utils::SplitString(shaderText, SHADER_DELIMITER);

		if (shaderCodes.size() != 2)
		{
			TWISTED_WARN("Invalid shader file: " + assetPath.string());
			return nullptr;
		}

		std::shared_ptr<ShaderData> shaderData = std::make_shared<ShaderData>();
		shaderData->Name = assetPath.stem().string();
		shaderData->VertShaderCode = shaderCodes[0];
		shaderData->FragShaderCode = shaderCodes[1];

		return shaderData;
	}

	std::shared_ptr<MeshData> ImportMeshData(const std::filesystem::path& assetPath)
	{
		Assimp::Importer importer;
		// Read the file with some post-processing
		const aiScene* scene = importer.ReadFile(assetPath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

		// Check if the import failed
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		{
			TWISTED_WARN("Mesh import failure: " +std::string(importer.GetErrorString()));
			return nullptr;
		}

		//MeshData meshData{ name,vertices,indices };
		//meshData.Name = name;
		//meshData.Vertices = vertices;
		//meshData.Indices = indices;

		//std::shared_ptr<Mesh> mesh = RenderAPI::CreateMesh(meshData);
		//m_meshes[meshData.Name] = mesh;


		return nullptr;
	}




	std::shared_ptr<MaterialData> ImportMaterial(const std::filesystem::path& assetPath)
	{
		return nullptr;
	}


	//for (auto& asset : allAssets)
	//{
	   // //meta exists
	   // //... internal exists    -> check meta
	   // //... internal not exist -> check meta + create internal
	   // //meta not exist
	   // //...internal exist      -> create meta
	   // //...internal not exist  -> create meta + create internal
	//}

}
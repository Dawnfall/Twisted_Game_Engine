#include "twistedpch.h"
#include "ResourceManager.h"
#include "AppCore.h"
#include "Utils/MeshCollections.h"
#include "Utils/FileUtils.h"
#include "Constants.h"
#include "Rendering/Model.h"

namespace Twisted
{
	const SRef<Texture> ResourceManager::GetTexture(const std::filesystem::path& path)
	{
		auto texture = m_textures.find(path.string());
		if (texture != m_textures.end())
			return texture->second;

		auto texData = TextureData::ImportTextureData(path);
		if (!texData)
			return nullptr;

		SRef<Texture> tex = Texture::CreateTexture(*texData);
		if (tex)
			m_textures[path.string()] = tex;
		return tex;
	}
	const SRef<Shader> ResourceManager::GetShader(const std::filesystem::path& path)
	{
		auto shader = m_shaders.find(path.string());
		if (shader != m_shaders.end())
			return shader->second;

		auto shaderData = ShaderData::ImportShaderData(path);
		if (!shaderData)
			return nullptr;

		SRef<Shader> shad = std::make_shared<Shader>(*shaderData);
		if (!shad)
			return nullptr;

		m_shaders[path.string()] = shad;
		return shad;
	}
	SRef<Model> ResourceManager::GetModel(const std::filesystem::path& path)
	{
		auto model = m_models.find(path.string());
		if (model != m_models.end())
			return model->second;

		auto result = rapidobj::ParseFile(path);
		if (result.error)
		{
			TWISTED_WARN(result.error.code.message());
			return nullptr;
		}
		if (!rapidobj::Triangulate(result))
		{
			TWISTED_WARN(result.error.code.message());
			return nullptr;
		}

		std::vector<SRef<Model::MaterialData>> materialDatas;
		for (const auto& material : result.materials)
		{
			SRef<Model::MaterialData> materialData = std::make_shared<Model::MaterialData>();
			materialData->Name = material.name;
			if (!material.diffuse_texname.empty())
			{
				std::filesystem::path texturePath = path.parent_path() / material.diffuse_texname;
				auto texture = GetTexture(texturePath);
				if (texture)
					materialData->Diffuse = texture;
			}
			materialDatas.emplace_back(materialData);
		}

		std::vector<SRef<Mesh>> meshes;
		bool isUVs = !result.attributes.texcoords.empty();
		bool isNormals = !result.attributes.normals.empty();
		bool isColors = !result.attributes.colors.empty();

		for (const auto& shape : result.shapes)
		{
			MeshData meshData;
			for (const auto& index : shape.mesh.indices)
			{
				Vertex v;
				v.Position = {
					result.attributes.positions[3 * index.position_index + 0],
					result.attributes.positions[3 * index.position_index + 1],
					result.attributes.positions[3 * index.position_index + 2]
				};
				if (isNormals)
				{
					v.Normal = {
						result.attributes.normals[3 * index.normal_index + 0],
						result.attributes.normals[3 * index.normal_index + 1],
						result.attributes.normals[3 * index.normal_index + 2]
					};
				}
				if (isUVs)
				{
					v.TexCoord = {
						result.attributes.texcoords[2 * index.texcoord_index + 0],
						result.attributes.texcoords[2 * index.texcoord_index + 1],
					};
				}
				meshData.Name = shape.name;
				meshData.Vertices.emplace_back(v);
				meshData.Indices.emplace_back(meshData.Indices.size());

				auto mesh = std::make_shared<Mesh>(meshData);
				if (!mesh)
				{
					TWISTED_WARN("Cannot create mesh from obj shape");
					continue;
				}
				meshes.emplace_back(mesh); //TODO...

				Model::ModelNode node;
				node.Name = shape.name;
				node.Mesh = mesh;

				if (!shape.mesh.material_ids.empty())
				{

				}
				int material_id = shape.mesh.material_ids[0];
				node.Material = materialDatas[material_id];
			}
		}
	}

	/*void ResourceManager::LoadAssets(const std::filesystem::path & assetsFolder)
	{*/
	//auto allAssetFiles = Utils::LoadFiles(std::filesystem::path(assetsFolder), "");
	//for (auto& file : allAssetFiles)
	//{
	//	SRef<Asset> newAsset = AssetImporter::ImportAsset(file);
	//	if (newAsset)
	//		m_assets[file.stem().string()] = newAsset;
	//}
	//}

	//const SRef<Material> ResourceManager::GetMaterial(const std::string matrialName)
	//{
	//	if (m_materials.contains(matrialName))
	//		return m_materials[matrialName];
	//	return nullptr;
	//}






	//SRef<Material> ResourceManager::CreateNewMaterial(const std::string & name)
	//{
	//	if (m_materials.contains(name))
	//	{
	//		//TODO....DAWN_WARN("Cannot create material: " + name + " ; Name already exists");
	//		return nullptr;
	//	}
	//	m_materials[name] = std::make_shared<Material>();
	//	return m_materials[name];
	//}

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

//SRef<Asset> ImportAsset(const std::filesystem::path& assetPath)
//{
//	//std::string ext = assetPath.extension().string();
//	//if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".tga" || ext == "bmp" || ext == ".psd"); //TODO....
//	//{
//	//	return TextureData::ImportTextureData(assetPath);
//	//}
//	//if (ext == ".shader")
//	//{
//	//	return ImportShaderData(assetPath);
//	//}
//	//if (ext == ".obj")
//	//{
//	//	return ImportMeshData(assetPath);
//	//}
//	//TWISTED_WARN("Asset not supported: " + assetPath.filename().string());
//	return nullptr;
//}




//for (auto& asset : allAssets)
//{
   // //meta exists
   // //... internal exists    -> check meta
   // //... internal not exist -> check meta + create internal
   // //meta not exist
   // //...internal exist      -> create meta
   // //...internal not exist  -> create meta + create internal
//}
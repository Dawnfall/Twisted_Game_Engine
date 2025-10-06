#pragma once
#include "Twisted/AssetsLayer/AssetImporter.h"
#include "Utils/Utils.h"
namespace Twisted
{
	const std::string ASSET_MODEL_TYPE = "model";

	class TWISTED_API ModelImporter :public AssetImporter
	{
	public:

		std::vector<fs::path> GetAssetExtensions()const override
		{
			return { ".obj" };
		}
		inline bool ImportOnStart()const { return true; }

		std::vector<TObject*>& Import(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const override;

		void PostImport(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const override;

		void FillDefaultInfo(YAML::Node& node)const override
		{
			node[ASSET_TYPE_KEY] = ASSET_MODEL_TYPE;
		}

	private:

	};
}


//TempModel model;
		//model.Path = (project.GetAssetsFolder() / relAssetPath).string();
		//std::vector<TempMesh> meshes;
		//std::vector<TempMaterial> materials;

		// Load materials
		//for (const auto& objMat : result.materials)
		//	materials.emplace_back(collectMaterial(objMat));

		// For each shape (mesh)
		//for (const auto& shape : result.shapes)
		//{
			/*const auto& mesh = shape.mesh;
			const auto& indices = mesh.indices;
			const auto& material_ids = mesh.material_ids;
			const auto& num_face_vertices = mesh.num_face_vertices;*/ //all 3 after triangulation

			//	// Group faces by material
			//	std::unordered_map<int, std::vector<size_t>> matToFaces;
			//	for (size_t faceIdx = 0; faceIdx < num_face_vertices.size(); ++faceIdx)
			//	{
			//		int matId = material_ids.empty() ? -1 : material_ids[faceIdx];
			//		matToFaces[matId].emplace_back(faceIdx);
			//	}

			//	// Handle each submesh
			//	int subMeshIndex = 0;
			//	size_t indexOffset = 0;
			//	for (const auto& [matId, faces] : matToFaces)
			//	{
			//		TempMesh meshData;
			//		meshData.Name = shape.name + "_" + std::to_string(subMeshIndex++);
			//		meshData.UUID = Utils::GenerateGUID();
			//		for (size_t faceIdx : faces)
			//		{
			//			int fv = num_face_vertices[faceIdx];
			//			for (int v = 0; v < fv; ++v)
			//			{
			//				meshData.Indices.emplace_back(static_cast<unsigned int>(meshData.Vertices.size()));
			//				meshData.Vertices.emplace_back(collectVertex(result.attributes, indices[indexOffset + v]));
			//			}
			//			indexOffset += fv;
			//		}
			//		meshes.emplace_back(meshData);
			//		WriteMesh(meshData);

			//		model.MeshUUIDs.emplace_back(meshData.UUID);
			//		if (matId >= 0)
			//		{
			//			materials.emplace_back(materials[matId].UUID);
			//			model.MatUUIDs.emplace_back(materials[matId].UUID);
			//		}
			//		else
			//		{
			//			materials.emplace_back("");
			//			model.MatUUIDs.emplace_back("");
			//		}
			//	}
			//	WriteModel(model);

			//}

			//for (auto& material : materials)
			//{

			//}
			//for (auto& mesh : meshes)
			//{

			//}






//void* ImportModelData(RegisterLayer& registerLayer, const std::filesystem::path& path)
//{
//	rapidobj::Result result = rapidobj::ParseFile(path);
//	if (result.error) {
//		TWISTED_WARN("Failed to load obj: " + path.string());
//		return nullptr;
//	}
//
//	rapidobj::Triangulate(result);
//
//	//	ModelData* modelData = assetsLayer.CreateObject<Twisted::ModelData>(path, result.shapes.size());
//
//	//Convert rapidobj data to your MeshData
//	for (const auto& shape : result.shapes)
//	{
//		std::vector<Vertex> vertices;
//		std::vector<unsigned int> indices;
//
//		for (const auto& index : shape.mesh.indices)
//		{
//			Vertex vertex{};
//
//			//Position
//			if (index.position_index >= 0) {
//				auto& pos = result.attributes.positions;
//				int pi = index.position_index * 3;
//				vertex.Position = glm::vec3(
//					pos[pi + 0],
//					pos[pi + 1],
//					pos[pi + 2]
//				);
//			}
//
//			//Normal
//			if (index.normal_index >= 0) {
//				auto& norm = result.attributes.normals;
//				int ni = index.normal_index * 3;
//				vertex.Normal = glm::vec3(
//					norm[ni + 0],
//					norm[ni + 1],
//					norm[ni + 2]
//				);
//			}
//
//			//TexCoord
//			if (index.texcoord_index >= 0) {
//				auto& tex = result.attributes.texcoords;
//				int ti = index.texcoord_index * 2;
//				vertex.TexCoord = glm::vec2(
//					tex[ti + 0],
//					tex[ti + 1]
//				);
//			}
//
//			vertices.emplace_back(vertex);
//			indices.emplace_back(vertices.size() - 1);
//
//			Mesh* mesh = registerLayer.CreateObject<Mesh>(vertices, indices); //TODO... primitive type
//		}
//	}
//	return nullptr;
//	//	return modelData;
//}

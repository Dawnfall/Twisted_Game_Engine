#include "ModelImporter.h"

#include "Utils/GlmUtils.h"
#include "Data/Vertex.h"

#include <rapidobj/rapidobj.hpp>
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"

namespace Twisted
{
	struct TempModel
	{
		std::string Name;
		std::string UUID;
		std::string Path;
		std::vector<std::string> MeshUUIDs;
		std::vector<std::string> MatUUIDs;
	};

	struct TempMaterial
	{
		std::string Name;
		std::string UUID;
		std::unordered_map<std::string, std::string> TexMap;
	};

	struct TempMesh
	{
		std::string Name;
		std::string UUID;
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
	};

	TempMaterial collectMaterial(const rapidobj::Material& objMaterial)
	{
		TempMaterial mat;
		mat.Name = objMaterial.name;
		//mat.UUID = Utils::GenerateGUID();

		if (!objMaterial.diffuse_texname.empty())
			mat.TexMap["diffuse"] = objMaterial.diffuse_texname;
		//TODO...

		return mat;
	}

	Vertex collectVertex(const rapidobj::Attributes& attributes, const rapidobj::Index& idx)
	{
		Vertex vertex{};
		// Position
		if (idx.position_index >= 0) {
			auto& pos = attributes.positions;
			int pi = idx.position_index * 3;
			vertex.Position = glm::vec3(
				pos[pi + 0],
				pos[pi + 1],
				pos[pi + 2]
			);
		}
		// Normal
		if (idx.normal_index >= 0) {
			auto& norm = attributes.normals;
			int ni = idx.normal_index * 3;
			vertex.Normal = glm::vec3(
				norm[ni + 0],
				norm[ni + 1],
				norm[ni + 2]
			);
		}
		// TexCoord
		if (idx.texcoord_index >= 0) {
			auto& tex = attributes.texcoords;
			int ti = idx.texcoord_index * 2;
			vertex.TexCoord = glm::vec2(
				tex[ti + 0],
				tex[ti + 1]
			);
		}
		return vertex;
	}

	void ModelImporter::ImportNew(AssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		//TODO:...
	}
	void ModelImporter::HotReload(AssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		//TODO:...
	}

	void ModelImporter::PostImport(AssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		//std::vector<std::string> MaterialUuids;
		//std::vector<std::string> MeshUuids;

		/*rapidobj::Result result = rapidobj::ParseFile(GetAssetPath());
		if (result.error) {
			TWISTED_WARN("Failed to load obj: " + GetAssetPath().string());
			return;
		}

		rapidobj::Triangulate(result);

		for (int i = 0; i < result.materials.size(); i++)
			Node["materials"].push_back(Utils::GenerateGUID());

		for (const auto& shape : result.shapes)
		{
			std::unordered_map<int, std::vector<size_t>> matToFaces;
			for (size_t faceIdx = 0; faceIdx < shape.mesh.num_face_vertices.size(); ++faceIdx)
			{
				int matId = shape.mesh.material_ids.empty() ? -1 : shape.mesh.material_ids[faceIdx];
				matToFaces[matId].emplace_back(faceIdx);
			}

			for (int i = 0; i < matToFaces.size(); i++)
				Node["meshes"].push_back(Utils::GenerateGUID());
		}*/

		//AssetObjects objects;
		//return objects;
	}
}

REGISTER_IMPORTER(ModelImporter)


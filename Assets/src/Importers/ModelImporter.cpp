#include "Importers/ModelImporter.h"
#include "Importers/MaterialImporter.h"
#include "AssetImporterRegistry.h"
#include "ModelAsset.h"
#include "AssetsService.h"
#include "AssetUuidYaml.h"
#include "Application/Application.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Data/MeshData.h"
#include "Data/ShaderData.h"
#include "Application/TObject.h"
#include "Utils/WPtr.h"
#include "Debug/Logger.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Twisted
{
	static bool TryLinkDiffuseTexture(
		const aiMaterial* aiMat,
		const fs::path& modelPath,
		Material* material,
		AssetsService* assetsService)
	{
		aiString texPath;
		if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) != AI_SUCCESS)
			return false;

		fs::path fullPath = modelPath.parent_path() / texPath.C_Str();
		if (!fs::exists(fullPath))
			return false;

		ImportedAsset* texAsset = assetsService->RegisterAsset(fullPath);
		if (!texAsset)
			return false;

		AssetUuid texUuid = texAsset->GetUuid();
		assetsService->Load(texUuid);
		Texture* tex = assetsService->GetObject<Texture>(texUuid);
		if (!tex)
			return false;

		// Assign to the first unoccupied texture slot — skip if already linked
		for (auto& mv : material->GetValues())
		{
			if (auto* tv = std::get_if<TextureValue>(&mv.val))
			{
				if (tv->tex)
					return false;
				tv->tex = tex;
				return true;
			}
		}
		return false;
	}

	static std::string SanitizeName(const std::string& name)
	{
		std::string result = name;
		for (char& c : result)
			if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_' && c != '-')
				c = '_';
		return result;
	}

	static WPtr<Mesh> BuildMesh(const aiMesh* aiMesh, const fs::path& fileStem, size_t index)
	{
		MeshData meshData;

		for (unsigned int v = 0; v < aiMesh->mNumVertices; v++)
		{
			meshData.AddPosition({ aiMesh->mVertices[v].x, aiMesh->mVertices[v].y, aiMesh->mVertices[v].z });

			if (aiMesh->HasNormals())
				meshData.AddNormal({ aiMesh->mNormals[v].x, aiMesh->mNormals[v].y, aiMesh->mNormals[v].z });

			meshData.AddTexCoord(aiMesh->HasTextureCoords(0)
				? glm::vec2{ aiMesh->mTextureCoords[0][v].x, aiMesh->mTextureCoords[0][v].y }
				: glm::vec2{ 0.0f, 0.0f });
		}

		for (unsigned int f = 0; f < aiMesh->mNumFaces; f++)
		{
			const aiFace& face = aiMesh->mFaces[f];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				meshData.AddIndex(face.mIndices[j]);
		}

		if (!aiMesh->HasNormals())
			meshData.RecalculateNormals();

		std::string meshName = aiMesh->mName.length > 0
			? std::string(aiMesh->mName.C_Str())
			: fileStem.string() + "_" + std::to_string(index);

		WPtr<Mesh> mesh(TObject::Create<Mesh>(meshName));
		mesh->SetData(meshData.Pack(), MeshDrawType::STATIC);
		return mesh;
	}

	static glm::mat4 ToGlm(const aiMatrix4x4& t)
	{
		return glm::mat4(
			t.a1, t.b1, t.c1, t.d1,
			t.a2, t.b2, t.c2, t.d2,
			t.a3, t.b3, t.c3, t.d3,
			t.a4, t.b4, t.c4, t.d4
		);
	}

	static ModelNode BuildNode(
		const aiScene* scene,
		const aiNode* node,
		const std::vector<WPtr<Mesh>>& sceneMeshes,
		const std::vector<WPtr<Material>>& sceneMaterials)
	{
		ModelNode modelNode;
		modelNode.name           = node->mName.C_Str();
		modelNode.localTransform = ToGlm(node->mTransformation);

		if (node->mNumMeshes > 0)
		{
			unsigned int meshIdx = node->mMeshes[0];
			modelNode.mesh       = sceneMeshes[meshIdx];

			unsigned int matIdx  = scene->mMeshes[meshIdx]->mMaterialIndex;
			if (matIdx < sceneMaterials.size())
			{
				aiString slotName;
				scene->mMaterials[matIdx]->Get(AI_MATKEY_NAME, slotName);
				modelNode.materialSlots.push_back({ slotName.C_Str(), sceneMaterials[matIdx] });
			}

			for (unsigned int i = 1; i < node->mNumMeshes; i++)
			{
				unsigned int extraIdx   = node->mMeshes[i];
				unsigned int extraMatIdx = scene->mMeshes[extraIdx]->mMaterialIndex;

				ModelNode extraNode;
				extraNode.name           = modelNode.name + "_" + std::to_string(i);
				extraNode.localTransform = glm::mat4(1.0f);
				extraNode.mesh           = sceneMeshes[extraIdx];

				if (extraMatIdx < sceneMaterials.size())
				{
					aiString slotName;
					scene->mMaterials[extraMatIdx]->Get(AI_MATKEY_NAME, slotName);
					extraNode.materialSlots.push_back({ slotName.C_Str(), sceneMaterials[extraMatIdx] });
				}

				modelNode.children.push_back(std::move(extraNode));
			}
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
			modelNode.children.push_back(BuildNode(scene, node->mChildren[i], sceneMeshes, sceneMaterials));

		return modelNode;
	}

	SRef<FileImportedAsset> ModelImporter::Load(const fs::path& path) const
	{
		auto asset = std::make_shared<ModelAsset>(path);

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
			path.string(),
			aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs
		);

		if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
		{
			TWISTED_WARN("Failed to load model: {} ; {}", path.string(), std::string(importer.GetErrorString()));
			return asset;
		}

		std::vector<WPtr<Mesh>> sceneMeshes;
		sceneMeshes.reserve(scene->mNumMeshes);
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			WPtr<Mesh> mesh = BuildMesh(scene->mMeshes[i], path.stem(), i);
			sceneMeshes.push_back(mesh);
			asset->GetObjects().push_back(mesh);
		}

		// Build material assets — create .material files next to the model and register them.
		AssetsService* assetsService = Application::GetInstance().GetService<AssetsService>();

		fs::path matDir = path.parent_path() / "Materials";
		if (!fs::exists(matDir))
			fs::create_directories(matDir);

		YAML::Node& info = asset->GetInfo();
		bool infoModified = false;

		std::vector<WPtr<Material>> sceneMaterials;
		sceneMaterials.reserve(scene->mNumMaterials);

		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			aiString aiName;
			scene->mMaterials[i]->Get(AI_MATKEY_NAME, aiName);
			std::string matName = aiName.length > 0
				? std::string(aiName.C_Str())
				: path.stem().string() + "_Mat_" + std::to_string(i);

			fs::path matPath = matDir / (path.stem().string() + "_" + SanitizeName(matName) + ".material");

			// Try to reuse existing material from a previous import
			AssetUuid matUuid = AssetUuid::Invalid();
			if (info["materials"] && info["materials"][matName])
				matUuid = info["materials"][matName].as<AssetUuid>(AssetUuid::Invalid());

			Material* matPtr = nullptr;

			if (matUuid.IsValid() && assetsService->IsLoaded(matUuid))
			{
				matPtr = assetsService->GetObject<Material>(matUuid);
			}
			else
			{
				// Create the .material file on first import
				bool isFirstImport = !fs::exists(matPath);
				if (isFirstImport)
				{
					MaterialImporter matImporter;
					matImporter.CreateNew(matPath);
				}

				// Register and load via AssetsService so it gets a UUID and lands in m_assets
				ImportedAsset* matAsset = assetsService->RegisterAsset(matPath);
				if (matAsset)
				{
					matUuid = matAsset->GetUuid();
					assetsService->Load(matUuid);
					matPtr = assetsService->GetObject<Material>(matUuid);

					if (matPtr)
					{
						bool textureLinked = TryLinkDiffuseTexture(scene->mMaterials[i], path, matPtr, assetsService);
						if (textureLinked)
							assetsService->Save(matUuid);
					}

					info["materials"][matName] = matUuid;
					infoModified = true;
				}
			}

			sceneMaterials.push_back(matPtr ? WPtr<Material>(matPtr) : WPtr<Material>(nullptr));
		}

		if (infoModified)
			asset->SaveInfo();

		asset->GetRootNode() = BuildNode(scene, scene->mRootNode, sceneMeshes, sceneMaterials);

		return asset;
	}

	void ModelImporter::HotReload(FileImportedAsset& asset) const
	{
		(void)asset;
		// TODO
	}
}

REGISTER_IMPORTER(ModelImporter)

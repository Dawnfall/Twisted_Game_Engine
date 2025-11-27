#pragma once
#include "Twisted/Application/Service.h"

#include "AppCore.h"
#include "Twisted/AssetsLayer/AssetImporter.h"
#include "AssetImporterRegistry.h"
#include "AssetInfo.h"

#include "Utils/WPtr.h"

#include <unordered_map>
#include <filesystem>

#include "Twisted/TObject.h"
#include "Twisted/Constants.h"
#include "Project.h"

#include "Twisted/Rendering/Mesh.h"
#include "Twisted/BuiltIn/MeshCollections.h"
#include "Twisted/Rendering/OpenGL/Mesh_OpenGL.h"

#include "Twisted/Rendering/Shader.h"
#include "Twisted/BuiltIn/ShaderCollections.h"
#include "Twisted/Rendering/OpenGL/Shader_OpenGL.h"

namespace Twisted
{
	class TWISTED_API AssetsService :public Service
	{
	public:
		AssetsService(Application* app) :Service(app) { s_instance = this; }

		inline static AssetsService* GetInstance()
		{
			return s_instance;
		}

		AssetInfo* GetInfo(const fs::path& assetPath)const;
		AssetInfo* GetInfo(const AssetUuid& uuid)const;
		AssetInfo* GetObjectAssetInfo(const TObject* object)const;
		TObject* GetAssetObject(AssetUuid uuid, const std::string& name);

		void AutoImportAssets();
		std::vector<WPtrBase> ImportAssetDirect(const fs::path& assetPath)const;

		template<typename T>
		std::vector<T*> GetObjectsOfType()
		{
			std::vector<T*> objects;
			for (auto& [uuid, assetObjects] : m_assetObjects)
			{
				for (auto& assetObj : assetObjects)
				{
					T* castObj = dynamic_cast<T*>(assetObj.GetObj());
					if (castObj)
						objects.emplace_back(castObj);
				}
			}
			return objects;
		}

		Project& GetProject() { return m_project; }
		const Project& GetProject()const { return m_project; }

		std::vector<WPtrBase>& GetManagedAssetObjects(AssetInfo* info);
		const std::vector<WPtrBase>& GetManagedAssetObjects(AssetInfo* info)const;

		//inline void OnInit()override
		//{
		//	////resource load
		//	//GetProject().ProjectChangeEvent.AddListener([this]() {
		//	//	LoadBuiltIn();
		//	//	AutoImportAssets();
		//	//	});
		//}

		void AddBuiltIn(AssetUuid uuid, TObject* obj);

		void SaveAsset(const FileAssetInfo* info, const std::vector<WPtrBase>& objects);

		bool CreateNewAsset(const fs::path& path);

		//for non auto importer assets
		void SaveAssetDirect(const fs::path& assetPath, const std::vector<WPtrBase>& objects);

		void SaveAssetManaged(FileAssetInfo* info);

		void LoadBuiltIn()
		{
			Mesh* triangleMesh = TObject::Create<Mesh>(Collections::triangleMeshName);
			Mesh_GL::SetData(*triangleMesh, Collections::CreateTrianglePackedData(), MeshDrawType::STATIC);

			Mesh* quadMesh = TObject::Create<Mesh>(Collections::quadMeshName);
			Mesh_GL::SetData(*quadMesh, Collections::CreateQuadPackedData(), MeshDrawType::STATIC);

			Mesh* cubeMesh = TObject::Create<Mesh>(Collections::cubeMeshName);
			Mesh_GL::SetData(*cubeMesh, Collections::CreateCubePackedData(), MeshDrawType::STATIC);

			Shader* debugUVsShader = TObject::Create<Shader>(Collections::debugUVsShaderName);
			ShaderData debugUVsShaderData;
			debugUVsShaderData.VertShader = Collections::debugUVsVertexShader;
			debugUVsShaderData.FragShader = Collections::debugUVsFragmentShader;
			Shader_GL::SetData(*debugUVsShader, debugUVsShaderData);

			Shader* debugNormalsShader = TObject::Create<Shader>(Collections::debugNormalsShaderName);
			ShaderData debugNormalsShaderData;
			debugNormalsShaderData.VertShader = Collections::debugNormalsVertexShader;
			debugNormalsShaderData.FragShader = Collections::debugNormalsFragmentShader;
			Shader_GL::SetData(*debugNormalsShader, debugNormalsShaderData);

			Shader* defaultShader = TObject::Create<Shader>(Collections::defaultShaderName);
			ShaderData defaultShaderData;
			defaultShaderData.VertShader = Collections::defaultVertexShader;
			defaultShaderData.FragShader = Collections::defaultFragmentShader;
			Shader_GL::SetData(*defaultShader, defaultShaderData);

			Shader* simpleShader = TObject::Create<Shader>(Collections::simpleShaderName);
			ShaderData simpleShaderData;
			simpleShaderData.VertShader = Collections::simpleVertexShader;
			simpleShaderData.FragShader = Collections::simpleFragmentShader;
			Shader_GL::SetData(*simpleShader, simpleShaderData);

			AddBuiltIn(Collections::triangleMeshUUID, triangleMesh);
			AddBuiltIn(Collections::quadMeshUUID, quadMesh);
			AddBuiltIn(Collections::cubeMeshUUID, cubeMesh);

			AddBuiltIn(Collections::debugNormalsShaderUUID, debugNormalsShader);
			AddBuiltIn(Collections::debugUVsShaderUUID, debugUVsShader);
			AddBuiltIn(Collections::defaultShaderUUID, defaultShader);
			AddBuiltIn(Collections::simpleShaderUUID, simpleShader);
		}

	private:

		std::unordered_map<fs::path, SRef<AssetInfo>> m_assetsByPath;
		std::unordered_map<AssetUuid, SRef<AssetInfo>> m_assetsByUuid;
		std::unordered_map<AssetUuid, std::vector<WPtrBase>> m_assetObjects;

		inline static AssetsService* s_instance = nullptr;

	private:
		Project m_project;

		AssetInfo* CreateInfo(const fs::path& assetPath);
		void DetectAllAssets(const fs::path& assetsFolder);
		void DeleteLoneInfos(const fs::path& assetsFolder);
		void RemoveDanglingAssetObjects(const fs::path& assetsFolder);
		void ImportManaged(const std::vector<FileAssetInfo*>& infos);
		void RemoveAsset(FileAssetInfo* asset);


	};
}
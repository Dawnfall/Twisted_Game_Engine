#pragma once


#include "Debug/Logger.h"
#include "Twisted/Rendering/RenderingAPI.h"
#include "Twisted/Resources/ShaderData.h"
#include "Twisted/Rendering/Shader.h"
#include "Twisted/Resources/MeshData.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"
#include "Collections/Geometry.h"

//**************
// CONSTS

const std::string materialName = "default_material";

//**************

namespace Twisted
{
	class ResourceManager
	{
	public:
		void LoadResources(const std::string& projectFolder);
		
		void LoadTexture(const std::string& fileName, const std::string& name);
		void LoadMesh(const std::string& name,const std::vector<Vertex> vertices, std::vector<unsigned int> indices);

		std::shared_ptr<Material> CreateNewMaterial(const std::string& name);

		const std::shared_ptr<Material>& GetMaterial(const std::string matrialName);
		const std::shared_ptr<Shader>& GetShader(const std::string& shaderName);
		const std::shared_ptr<Texture>& GetTexture(const std::string& textureName);
		const std::shared_ptr<Mesh>& GetMesh(const std::string& meshName);

	private:
		void LoadDefaultResources();

		std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
		std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshes;
	};
}
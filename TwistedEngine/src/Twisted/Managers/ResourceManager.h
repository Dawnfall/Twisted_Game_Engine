#pragma once

#include "Debug/Logger.h"

#include "Twisted/Rendering/OpenGL/Render_OpenGL.h"

#include "Twisted/Resources/ShaderData.h"
#include "Twisted/Rendering/Shader.h"

#include "Twisted/Resources/MeshData.h"
#include "Twisted/Rendering/Mesh.h"

#include "Twisted/Rendering/Material.h"

#include "Collections/Geometry.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//**************
// CONSTS

const std::string materialName = "default_material";
const std::string shaderDelimiter = "---";

//**************

namespace Twisted
{
	class ResourceManager
	{
	public:
		void LoadResources(const std::string& projectFolder);
		
		void LoadShader(const std::string& shaderName,const std::string& vertexCode,const std::string& fragmentCode);
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
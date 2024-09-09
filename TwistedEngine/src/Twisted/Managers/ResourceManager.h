#pragma once

#include "Debug/Logger.h"

#include "Twisted/Rendering/OpenGL/Render_OpenGL.h"

#include "Twisted/Resources/ShaderData.h"
#include "Twisted/Rendering/Shader.h"

#include "Twisted/Resources/MeshData.h"
#include "Twisted/Rendering/Mesh.h"

#include "Twisted/Resources/Resource.h"
#include "Twisted/Rendering/Material.h"

#include "Collections/Geometry.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Twisted
{
	class ResourceManager
	{
	public:
		template<typename T>
		void AddResource(T& newRes)
		{
			m_resources[newRes.Name] = std::make_shared<T>(newRes);
		}
		template<typename T>
		const std::shared_ptr<Resource>& GetResource(const std::string& name)
		{
			if (m_resources.contains(name))
				return std::dynamic_pointer_cast<T>(m_resources[name]);
			return nullptr;
		}
		
		void CreateNewTexture(const std::string& filePath, const std::string& textureName = "");
		std::shared_ptr<Material> CreateNewMaterial(const std::string& name);

		const std::shared_ptr<Material>& GetMaterial(const std::string matrialName);
		const std::shared_ptr<Shader>& GetShader(const std::string& shaderName);
		const std::shared_ptr<Texture>& GetTexture(const std::string& textureName);

		const std::shared_ptr<Mesh>& GetMesh(const std::string& meshName);

		void CompileShaders();
		void CompileMeshes();

	private:
		std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

		std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
		std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshes;
	};
}
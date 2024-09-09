#include "pch.h"
#include "ResourceManager.h"

namespace Twisted
{
	void ResourceManager::CompileShaders()
	{
		for (auto& keyVal : m_resources)
		{
			auto shaderData = std::dynamic_pointer_cast<ShaderData>(keyVal.second);
			if (shaderData)
			{
				std::shared_ptr<Shader> shader = Render_OpenGL::CreateShader(shaderData);
				m_shaders[shaderData->Name] = shader;
			}
		}
	}

	void ResourceManager::CompileMeshes()
	{
		for (auto& keyVal : m_resources)
		{
			auto meshData = std::dynamic_pointer_cast<MeshData>(keyVal.second);
			if (meshData)
			{
				auto mesh = std::make_shared<Mesh>(meshData);
				Render_OpenGL::UploadMesh(mesh);
				m_meshes[meshData->Name] = mesh;
			}
		}
	}

	void ResourceManager::CreateNewTexture(const std::string& filePath, const std::string& textureName)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			std::shared_ptr<Texture> newTex = Render_OpenGL::LoadTexture(textureName, width, height, data);
			m_textures[textureName] = newTex;
		}

		stbi_image_free(data);
	}

	const std::shared_ptr<Material>& ResourceManager::GetMaterial(const std::string matrialName) {
		if (m_materials.contains(matrialName))
			return m_materials[matrialName];
		return nullptr;
	}
	const std::shared_ptr<Shader>& ResourceManager::GetShader(const std::string& shaderName)
	{
		if (m_shaders.contains(shaderName))
			return m_shaders[shaderName];
		return nullptr;
	}
	const std::shared_ptr<Texture>& ResourceManager::GetTexture(const std::string& textureName)
	{
		if (m_textures.contains(textureName))
			return m_textures[textureName];
		return nullptr;
	}

	const std::shared_ptr<Mesh>& ResourceManager::GetMesh(const std::string& meshName)
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
}
#include "pch.h"
#include "ResourceManager.h"
#include "Debug/Logger.h"
#include "Collections/Shaders/ShaderCollections.h"
#include "Collections/Meshes/MeshCollections.h"

namespace Twisted
{
	void ResourceManager::LoadResources(const std::string& projectFolder)
	{
		LoadDefaultResources();

		for (auto& entry : Utils::GetFilesInFolder(projectFolder))
		{
			std::string fileNameWithoutExtension = entry.path().stem().string();
			std::string fullPath = entry.path().string();
			std::string extension = entry.path().extension().string();
			std::string fileName = entry.path().stem().string();

			if (extension == ".shader")
			{
				std::string shaderText = Utils::ReadFileContent(fullPath);
				std::vector<std::string> shaderCodes = Utils::SplitString(shaderText, shaderDelimiter);
				if (shaderCodes.size() != 2)
				{
					TWISTED_WARN("Invalid shader file: " + fullPath);
					return;
				}
				LoadShader(fileName, shaderCodes[0], shaderCodes[1]);
			}
			else if (extension == ".jpg" || extension == ".png")
			{
				LoadTexture(fullPath, fileName);
			}
			else
			{
				TWISTED_WARN("Unknown file type: " + fullPath);
			}
		}
	}

	void ResourceManager::LoadDefaultResources()
	{
		LoadShader(Collections::defaultShaderName, Collections::simpleVertexCode, Collections::simpleFragmentCode);
		LoadMesh(Collections::triangleMeshName, Collections::triangleVertices, Collections::triangleIndices);
		LoadMesh(Collections::quadMeshName, Collections::quadVertices, Collections::quadIndices);
		LoadMesh(Collections::cubeMeshName, Collections::cubeVertices, Collections::cubeIndices);
	}

	void ResourceManager::LoadMesh(const std::string& name, std::vector<Vertex> vertices, std::vector<unsigned int> indices) 
	{
		MeshData meshData{ name,vertices,indices };
		meshData.Name = name;
		meshData.Vertices = vertices;
		meshData.Indices = indices;
	
		std::shared_ptr<Mesh> mesh = Render_OpenGL::CreateMesh(meshData);
		m_meshes[meshData.Name] = mesh;
	}

	void ResourceManager::LoadShader(const std::string& shaderName, const std::string& vertexCode, const std::string& fragmentCode)
	{
		ShaderData shaderData;
		shaderData.Name = shaderName;
		shaderData.VertShaderCode = vertexCode;
		shaderData.FragShaderCode = fragmentCode;

		std::shared_ptr<Shader> shader = Render_OpenGL::CreateShader(shaderData);
		m_shaders[shaderData.Name] = shader;
	}

	void ResourceManager::LoadTexture(const std::string& filePath, const std::string& name)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			std::shared_ptr<Texture> newTex = Render_OpenGL::LoadTexture(name, width, height, data);
			m_textures[name] = newTex;
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
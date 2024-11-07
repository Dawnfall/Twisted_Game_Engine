#include "twistedpch.h"
#include "ResourceManager.h"
#include "Debug/Logger.h"
#include "Collections/Meshes/MeshCollections.h"
#include "Twisted/Rendering/ShaderCompilation.h"
#include "Utils/FileUtils.h"

namespace Twisted
{
	const std::string DEFAULT_SHADER_PATH = "F:/Programiranje/C++/GameEngine/TwistedEngine/src/BuiltIn/Shaders/defaultShader.shader";
	const std::string DEFAULT_SHADER_NAME = "DefaultShader";
	const std::string SIMPLE_SHADER_PATH = "F:/Programiranje/C++/GameEngine/TwistedEngine/src/BuiltIn/Shaders/SimpleShader.shader";
	const std::string SIMPLE_SHADER_NAME = "SimpleShader";

	void ResourceManager::LoadResources(const std::string& projectFolder)
	{
		LoadDefaultResources();

		for (auto& entry : Utils::GetFilesInFolder(projectFolder))
		{
			std::string fileNameWithoutExtension = entry.path().stem().string();
			std::string fullPath = entry.path().string();
			std::string extension = entry.path().extension().string();
			std::string fileName = entry.path().stem().string();

			if (extension == ShaderCompilation::SHADER_EXTENSION)
			{
				auto shader = ShaderCompilation::LoadShader(fullPath, fileName);
				if (shader)
					m_shaders[shader->Data.Name] = shader;
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
		auto defaultShader = ShaderCompilation::LoadShader(DEFAULT_SHADER_PATH,DEFAULT_SHADER_NAME);
		if (defaultShader)
			m_shaders[defaultShader->Data.Name] = defaultShader;

		auto simpleShader = ShaderCompilation::LoadShader(SIMPLE_SHADER_PATH, SIMPLE_SHADER_NAME);
		if (simpleShader)
			m_shaders[simpleShader->Data.Name] = simpleShader;

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

		std::shared_ptr<Mesh> mesh = RenderAPI::CreateMesh(meshData);
		m_meshes[meshData.Name] = mesh;
	}

	void ResourceManager::LoadTexture(const std::string& filePath, const std::string& name)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			std::shared_ptr<Texture> newTex = RenderAPI::LoadTexture(name, width, height, data);
			m_textures[name] = newTex;
		}

		stbi_image_free(data);
	}

	const std::shared_ptr<Material>& ResourceManager::GetMaterial(const std::string matrialName)
	{
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
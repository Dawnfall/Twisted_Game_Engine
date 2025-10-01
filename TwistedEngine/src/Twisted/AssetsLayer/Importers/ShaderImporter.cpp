#include "ShaderImporter.h"
#include "Twisted/Rendering/Shader.h"
#include "Utils/FileUtils.h"
#include "Utils/Utils.h"

#include "Logger.h"

namespace Twisted
{
	AssetObjects ShaderImporter::CreateObjects(AssetObjects& currObjects)const
	{
		if (currObjects.size()>0)
			currObjects[0] = ObjectManager::GetInstance().ReloadObject<Shader>(currObjects[0])->getID();
		else
			currObjects.emplace_back(ObjectManager::GetInstance().CreateObject<Shader>()->getID());

		return currObjects;
	}

	void ShaderImporter::PostCreate(const AssetInfo& assetInfo, AssetObjects& objects, AssetsLayer* assetsLayer)const
	{
		ShaderData ShadData;

		std::string shaderText = Utils::ReadFileContent(assetInfo.AssetPath);
		std::vector<std::string> shaderCodes = Utils::SplitString(shaderText, SHADER_DELIMITER);

		if (shaderCodes.size() != 2)
		{
			TWISTED_WARN("Invalid shader file: " + assetInfo.AssetPath.string());
			return;
		}

		ShadData.VertShader = shaderCodes[0];
		ShadData.FragShader = shaderCodes[1];
		ShadData.GeoShader = "";

		Shader* shader = ObjectManager::GetInstance().GetIdObject<Shader>(objects[0]);
		//TODO....
	}
}
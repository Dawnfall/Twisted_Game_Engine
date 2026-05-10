#include "Utils/YamlUtils.h"
#include <fstream>

#include "Application/Application.h"
#include "Debug/Logger.h"

namespace YamlUtils
{
	bool saveNode(const YAML::Node& node, const std::filesystem::path& path, const std::string& errorMsg)
	{
		try
		{
			std::ofstream fout(path);
			fout << node;
			fout.close();
			return true;
		}
		catch (...)
		{
			TWISTED_WARN(errorMsg);
			return false;
		}
	}

	bool loadNode(const std::filesystem::path& path, YAML::Node& node, const std::string& errorMsg)
	{
		try
		{
			node = YAML::LoadFile(path.string());
			return true;
		}
		catch (...)
		{
			TWISTED_ERROR(errorMsg);
			return false;
		}
	}
}


#include "YamlUtils.h"
#include <fstream>

namespace YamlUtils
{
	bool saveNode(const YAML::Node& node, const std::filesystem::path& path)
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
			return false;
		}

	}
}
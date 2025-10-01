#include "YamlFile.h"

namespace Twisted
{

	YamlFile::YamlFile(const std::filesystem::path& path) :
		m_path(path)
	{
		if (!Utils::IsExisting(m_path))
			Save();
		else
			Load();
	}
	bool YamlFile::Load()
	{
		m_rootNode = YAML::LoadFile(m_path.string());
		return true;
	}
	bool YamlFile::Save()const
	{
		std::ofstream fout(m_path);
		fout << m_rootNode;
		fout.close();
		return true;
	}



}
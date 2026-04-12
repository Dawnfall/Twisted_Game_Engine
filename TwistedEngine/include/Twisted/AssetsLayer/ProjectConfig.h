#pragma once
#include "AppCore.h"
#include "Twisted/AssetsLayer/AssetUuid.h"

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace Twisted
{
	struct PanelState
	{
		std::string name;
		bool showing;
	};

	class TWISTED_API ProjectConfig
	{
	public:
		void Load(const fs::path& path);
		void Save() const;

		AssetUuid lastWorld;
		std::string imguiIni;
		std::vector<PanelState> panelStates;

	private:
		fs::path m_path;
	};
}

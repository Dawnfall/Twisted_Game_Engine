#pragma once
#include "AppCore.h"
#include "AssetUuid.h"
#include "Utils/GlmUtils.h"

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

		Vec4f ambientLight = Vec4f(1.0f, 1.0f, 1.0f, 0.05f);

	private:
		fs::path m_path;
	};
}

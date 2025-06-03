#pragma once
#include "AppCore.h"
#include <filesystem>

namespace Twisted::Editor
{
	const std::filesystem::path editorRootFolder = "F:/Programiranje/C++/GameEngine/EditorConfig"; //TODO:...

	class EditorConfig;
	class Configurator
	{
	public:
		void LoadConfig();
		void WriteConfig() const;

		SRef<EditorConfig> Data;
	};
}
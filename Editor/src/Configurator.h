#pragma once
#include "AppCore.h"

namespace Twisted::Editor
{
	const std::filesystem::path editorRootFolder = "F:\Programiranje\C++\GameEngine\EditorConfig"; //TODO:...

	class EditorApp;
	class EditorConfig;
	class Configurator
	{
	public:
		void LoadConfig();
		void WriteConfig(EditorApp* editor) const;

		SRef<EditorConfig> Data;
	};
}
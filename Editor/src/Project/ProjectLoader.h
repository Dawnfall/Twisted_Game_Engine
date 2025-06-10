#pragma once

#include <imgui.h>
#include "AppCore.h"
#include "Windowing/NativeUtils.h"
#include "Project/Project.h"
#include "Windowing/Window.h"
#include <string>

namespace Twisted::Editor
{
    class EditorRuntime;
	class ProjectLoader
	{
    private:
		const std::string editorConfigFolder;
		const std::string editorConfigFileName;

        EditorRuntime* m_editor;
		void Init();
		bool m_isInit = false;

	public:
		std::string mainDockSpaceLabel="";
		ImGuiWindowFlags window_flags;

        SRef<Project> SelectedProject = nullptr;
		ProjectLoader(EditorRuntime* editor);

		void Render();
	};
}
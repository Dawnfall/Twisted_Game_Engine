#pragma once
#include "editorpch.h"

namespace Twisted::Editor::ImguiAPI
{
	void Init(GLFWwindow* windowPointer);
	void SetFlags();
	void SetStyle();
	void Terminate();
	void Render();

	void StartFrame();
	void EndFrame();

}

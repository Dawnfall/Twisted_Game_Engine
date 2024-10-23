#pragma once
#include "Editor/EditorWindow.h"

namespace Twisted::Editor
{
	class TreeWindow :public EditorWindow
	{
	public:
		//virtual void RenderContent(Twisted::Application* app)override;
		virtual void RenderContent(Twisted::Application* app)override;

		virtual std::string GetName()override { return "Tree Window"; }
	};
}
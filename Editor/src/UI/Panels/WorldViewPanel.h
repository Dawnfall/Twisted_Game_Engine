#pragma once
#include "UI/EditorPanel.h"
#include "AppCore.h"
#include "Twisted/Rendering/RenderContext.h"

#include <string>

namespace rend = Twisted::Render;

namespace Twisted::Editor
{
	class WorldViewPanel :public EditorPanel
	{
	public:
		WorldViewPanel(EditorRuntime* editor);

		virtual void RenderContent()override;
		virtual std::string GetName()override { return "World View"; }

		rend::RenderContext* m_context = nullptr;
	};
}
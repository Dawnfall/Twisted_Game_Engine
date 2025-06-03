#pragma once
#include "EditorPanel.h"
#include "AppCore.h"
#include <string>
#include "Rendering/RenderContext.h"

namespace rend = Twisted::Render;

namespace Twisted::Editor
{
	class WorldViewPanel :public EditorPanel
	{
	public:
		WorldViewPanel(EditorLayer* editor);

		virtual void RenderContent()override;
		virtual std::string GetName()override { return "World View"; }

		rend::RenderContext m_context;
	};

}
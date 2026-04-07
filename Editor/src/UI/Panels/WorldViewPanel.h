#pragma once
#include "UI/EditorPanel.h"
#include "EditorApp/EditorService.h"
#include "Twisted/Application/Application.h"

namespace Twisted::Editor
{
	class WorldViewPanel :public EditorPanel
	{
	public:
		WorldViewPanel();
		void Init() override
		{	
			m_editorService = Application::GetInstance().GetService<EditorService>();
		}

		virtual void PaintContent()override;

	private:
		EditorService* m_editorService = nullptr;
	};
}

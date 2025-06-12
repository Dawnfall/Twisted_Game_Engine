#pragma once
#include "UI/EditorPanel.h"
#include "UI/Details/DetailsRenderer.h"
#include "EditorRuntime.h"
#include "Twisted/Gameing/Entity.h"

namespace Twisted
{
	class AComponent;
}

namespace Twisted::Editor
{
	class DetailsPanel :public EditorPanel
	{
	public:
		DetailsPanel(EditorRuntime* editor) :EditorPanel(editor) {}
		virtual void RenderContent()override;
		virtual std::string GetName()override { return "Details Panel"; }

	private:
		template<typename T>
		void RenderComponent()
		{
			EntityID selectedEntity = m_editor->GetSelectedEntity();
			T* component = m_editor->GetGameWorld()->template TryGetComponent<T>(selectedEntity);

			if (!component)
				return;

			DetailsRenderer<T> detailsRenderer;

			// 1. Centered component label
			std::string compName = detailsRenderer.GetName();
			float windowWidth = ImGui::GetContentRegionAvail().x;
			float labelWidth = ImGui::CalcTextSize(compName.c_str()).x;
			float labelPosX = (windowWidth - labelWidth) * 0.5f;
			if (labelPosX > 0.0f)
				ImGui::SetCursorPosX(labelPosX);
			ImGui::Text("%s", compName.c_str());

			detailsRenderer.RenderDetails(component);

			ImGui::Separator();
		}

	};
}
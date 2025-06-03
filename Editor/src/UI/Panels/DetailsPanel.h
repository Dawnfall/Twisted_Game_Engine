#pragma once
#include "EditorPanel.h"
#include "EditorLayer.h"
#include "UI/Details/DetailsRenderer.h"

namespace Twisted
{
	class AComponent;
}

namespace Twisted::Editor
{
	class DetailsPanel :public EditorPanel
	{
	public:
		DetailsPanel(EditorLayer* editor) :EditorPanel(editor) {}
		virtual void RenderContent()override;
		virtual std::string GetName()override { return "Details Panel"; }

	private:
		template<typename T>
		void RenderComponent()
		{
			EntityID selectedEntity = editor->GetSelectedEntity();
			T* component = editor->GetActiveWorld()->template TryGetComponent<T>(selectedEntity);

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
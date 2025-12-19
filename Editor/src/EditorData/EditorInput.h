#pragma once
#include <imgui.h>

namespace Twisted::Editor
{
	const float dragMinDistance = 5.0f;

	class EditorInput
	{
	public:
		void Update()
		{
			ImGuiIO& io = ImGui::GetIO();

			m_mousePostion = io.MousePos;
			m_isDragEnd = false;
			m_isDragStart = false;
			m_isClicked = false;
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !m_isMouseDown)
			{
				m_mouseButton = ImGuiMouseButton_Left;
				m_isMouseDown = true;
				m_lastClickPos = m_mousePostion;
			}
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !m_isMouseDown)
			{
				m_mouseButton = ImGuiMouseButton_Right;
				m_isMouseDown = true;
				m_lastClickPos = m_mousePostion;

			}

			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) || ImGui::IsMouseReleased(ImGuiMouseButton_Right))
			{
				m_isMouseDown = false;
				if (m_isDragging)
				{
					m_isDragEnd = true;
					m_isDragging = false;
				}
				else
				{
					m_isClicked = true;
				}
			}

			if (m_isMouseDown && !m_isDragging)
			{
				float dx = m_mousePostion.x - m_lastClickPos.x;
				float dy = m_mousePostion.y - m_lastClickPos.y;
				float distance = sqrtf(dx * dx + dy * dy);

				if (distance >= dragMinDistance)
				{
					m_isDragStart = true;
					m_isDragging = true;
				}
			}

		}

		bool IsStartDrag() const { return m_isDragStart; }
		bool IsDragging()const { return m_isDragging; }
		bool IsEndDrag()const { return m_isDragEnd; }

		bool IsClicked()const { return m_isClicked; }

	private:

		ImVec2 m_mousePostion = ImVec2(0, 0);
		ImVec2 m_lastClickPos = ImVec2(0, 0);

		bool m_isMouseDown = false;
		bool m_isDragStart = false;
		bool m_isDragging = false;
		bool m_isDragEnd = false;

		bool m_isClicked = false;
		ImGuiMouseButton_ m_mouseButton = ImGuiMouseButton_::ImGuiMouseButton_Left;
	};
}

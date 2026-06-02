#include "UI/Components/CTransformPainter.h"
#include "EditorApp/EditorRegistry.h"
#include "UI/ImguiExtensions.h"

namespace Twisted::Editor
{
	static Vec3f NormalizeEulerDeg(Vec3f e)
	{
		for (int i = 0; i < 3; ++i)
		{
			e[i] = std::fmod(e[i], 360.0f);
			if (e[i] < 0.0f) e[i] += 360.0f;
		}
		return e;
	}

	void CTransformPainter::Paint(void* obj)
	{
		TransformComponent* transform = static_cast<TransformComponent*>(obj);

		if (ImGui::BeginTable("TransformTable", 2))
		{
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 80.0f);
			ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted("Position");
			ImGui::TableSetColumnIndex(1);
			Im::Vec3DragField("##Position", &transform->LocalPos.x, 0.1f);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted("Rotation");
			ImGui::TableSetColumnIndex(1);

			Vec3f& rotationEuler = [&]() -> Vec3f&
			{
				auto it = m_eulerCache.find(transform);
				if (it == m_eulerCache.end())
					it = m_eulerCache.emplace(transform, NormalizeEulerDeg(glm::degrees(transform->GetLocalRotationEulerRad()))).first;

				// If quaternion was changed externally, resync the cache
				Quat fromCache = Quat(glm::radians(it->second));
				if (glm::abs(glm::dot(fromCache, transform->LocalRot)) < 0.9999f)
					it->second = NormalizeEulerDeg(glm::degrees(transform->GetLocalRotationEulerRad()));

				return it->second;
			}();

			if (Im::Vec3DragField("##Rotation", &rotationEuler.x, 1.0f, "%.2f"))
			{
				rotationEuler = NormalizeEulerDeg(rotationEuler);
				transform->LocalRot = Quat(glm::radians(rotationEuler));
			}

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted("Scale");
			ImGui::TableSetColumnIndex(1);
			Im::Vec3DragField("##Scale", &transform->LocalScale.x, 0.01f);

			ImGui::EndTable();
		}
	}

}

REGISTER_COMPONENT_PAINTER(CTransformPainter, TransformComponent)

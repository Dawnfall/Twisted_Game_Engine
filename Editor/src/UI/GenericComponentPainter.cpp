#include "UI/GenericComponentPainter.h"
#include "UI/ImguiExtensions.h"
#include "Utils/GlmUtils.h"
#include <cstring>

namespace Twisted::Editor
{
	GenericComponentPainter::GenericComponentPainter(const std::vector<Twisted::PropertyInfo>* props)
		: m_props(props) {}

	void GenericComponentPainter::Paint(void* obj)
	{
		if (!m_props) return;
		for (const Twisted::PropertyInfo& prop : *m_props)
		{
			void* ptr = prop.accessor(obj);
			switch (prop.type)
			{
			case Twisted::PropertyType::Float:
				ImGui::DragFloat(prop.displayName, static_cast<float*>(ptr));
				break;
			case Twisted::PropertyType::Int:
				ImGui::DragInt(prop.displayName, static_cast<int*>(ptr));
				break;
			case Twisted::PropertyType::Bool:
				ImGui::Checkbox(prop.displayName, static_cast<bool*>(ptr));
				break;
			case Twisted::PropertyType::String:
			{
				std::string& s = *static_cast<std::string*>(ptr);
				char buf[256];
				strncpy(buf, s.c_str(), sizeof(buf) - 1);
				buf[sizeof(buf) - 1] = '\0';
				if (ImGui::InputText(prop.displayName, buf, sizeof(buf)))
					s = buf;
				break;
			}
			case Twisted::PropertyType::Vec2f:
				ImGui::DragFloat2(prop.displayName, &(*static_cast<Vec2f*>(ptr))[0]);
				break;
			case Twisted::PropertyType::Vec3f:
				Im::Vec3DragField(prop.displayName, &(*static_cast<Vec3f*>(ptr))[0]);
				break;
			case Twisted::PropertyType::Vec4f:
				ImGui::ColorEdit4(prop.displayName, &(*static_cast<Vec4f*>(ptr))[0]);
				break;
			case Twisted::PropertyType::Quat:
			{
				auto& q = *static_cast<Quat*>(ptr);
				Vec3f euler = glm::degrees(glm::eulerAngles(q));
				if (Im::Vec3DragField(prop.displayName, &euler[0]))
					q = Quat(glm::radians(euler));
				break;
			}
			}
		}
	}
}

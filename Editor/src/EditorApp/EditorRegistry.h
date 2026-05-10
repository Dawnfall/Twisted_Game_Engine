#pragma once

#include "UI/ComponentPainter.h"
#include "UI/EditorPanel.h"

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <functional>
#include <type_traits>

#include "FrameBuffer.h"
#include "Utils/WPtr.h"
#include "WorldRegistry.h"

namespace Twisted::Editor
{
	class DetailsPainter
	{
	public:
		virtual void Paint(void* obj) = 0;
	};

	class EditorRegistry
	{
	public:
		template<typename ObjectType, typename PainterType>
		void RegisterDetailsPainter()
		{
			static_assert(std::is_base_of_v<DetailsPainter, PainterType>, "Painter must derive from DetailsPainter");
			m_detailRenderers[std::type_index(typeid(ObjectType))] = std::make_unique<PainterType>();
		}

		template<typename T,typename C>
		void RegisterComponentPainter()
		{
			static_assert(std::is_base_of_v<ComponentPainter, T>, "T must derive from ComponentPainter");
			static_assert(std::is_base_of_v<AComponent, C>, "C must derive from AComponent");
			m_componentRenderers[GetTypeName<C>()] = std::make_unique<T>();
		}

		template<typename T>
		void RegisterPanel()
		{
			static_assert(std::is_base_of<EditorPanel, T>::value, "T must be derived from EditorPanel");
			m_panels.emplace_back(std::make_unique<T>());
		}

		ComponentPainter* GetComponentPainter(const std::string& compName)const
		{
			if (auto it = m_componentRenderers.find(compName); it != m_componentRenderers.end())
				return it->second.get();
			return nullptr;
		}

		DetailsPainter* GetDetailsPainter(std::type_index type)
		{
			auto it = m_detailRenderers.find(type);
			if (it != m_detailRenderers.end())
				return it->second.get();
			return nullptr;
		}
		

		std::vector<URef<EditorPanel>> m_panels;

	private:

		std::unordered_map<std::type_index, URef<DetailsPainter>> m_detailRenderers;
		std::unordered_map<std::string, URef<ComponentPainter>> m_componentRenderers;

	public:
		static EditorRegistry& GetInstance()
		{
			static EditorRegistry instance;
			return instance;
		}

		EditorRegistry(const EditorRegistry& other) = delete;
		EditorRegistry(EditorRegistry&& other) = delete;
		EditorRegistry& operator=(const EditorRegistry& other) = delete;
		EditorRegistry& operator=(EditorRegistry&& other) = delete;

	private:
		EditorRegistry() = default;
	};
}

#define REGISTER_COMPONENT_PAINTER(P,C)                                       \
namespace Registry                                                                                  \
{                                                                                                   \
    struct P##AutoRegister                                          \
    {                                                                         \
        P##AutoRegister()                                          \
        {                                                                     \
          Twisted::Editor::EditorRegistry::GetInstance().RegisterComponentPainter<Twisted::Editor::P,Twisted::C>();   \
        }                                                                     \
    } s_##P##AutoRegister;                                    \
}


#define REGISTER_EDITOR_PANEL(type)        \
namespace Registry                 \
{                                          \
    struct type##Register                  \
    {                                      \
        type##Register()                      \
        {                                     \
            Twisted::Editor::EditorRegistry::GetInstance().RegisterPanel<Twisted::Editor::type>(); \
        }                                     \
    };                                        \
    static type##Register s_##type##Register; \
}

#define REGISTER_DETAILS_PAINTER(objType,painterType)     \
namespace Registry                  \
{                                          \
    struct painterType##Register                  \
    {                                      \
        painterType##Register()                   \
        {                                  \
            Twisted::Editor::EditorRegistry::GetInstance().RegisterDetailsPainter<Twisted::objType,Twisted::Editor::painterType>(); \
        }                                  \
    };                                     \
    static painterType##Register s_##painterType##Register; \
}

#pragma once

#include "UI/DetailsPainter.h"
#include "UI/ComponentPainter.h"
#include "UI/EditorPanel.h"

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <functional>
#include <type_traits>

#include "Twisted/Rendering/FrameBuffer.h"
#include "Utils/WPtr.h"

namespace Twisted::Editor
{
	class EditorRegistry
	{
	public:
		static EditorRegistry& GetInstance()
		{
			static EditorRegistry instance;
			return instance;
		}

		template<typename T>
		void RegisterDetailsPainter()
		{
			static_assert(std::is_base_of_v<DetailsPainter, T>, "T must derive from DetailsRenderer!");
			DetailsRenderers[std::type_index(typeid(T))] = std::make_unique<T>();
		}

		template<typename T>
		T* GetDetailsPainter()
		{
			auto it = DetailsRenderers.find(std::type_index(typeid(T)));
			if (it != DetailsRenderers.end())
				return static_cast<T*>(it->second.get());
			return nullptr;
		}

		template<typename T>
		void RegisterComponentPainter()
		{
			static_assert(std::is_base_of_v<ComponentPainter, T>, "T must derive from ComponentPainter");
			CompPainters.emplace_back<URef<T>>(std::make_unique<T>());
		}


		template<typename T>
		void RegisterPanel()
		{
			static_assert(std::is_base_of<EditorPanel, T>::value, "T must be derived from EditorPanel");
			m_panels.emplace_back(std::make_unique<T>());
		}

		std::unordered_map<std::type_index, URef<DetailsPainter>> DetailsRenderers;
		std::vector<URef<ComponentPainter>> CompPainters;
		std::vector<URef<EditorPanel>> m_panels;

	private:
		EditorRegistry() = default;
		EditorRegistry(const EditorRegistry& other) = delete;
		EditorRegistry(EditorRegistry&& other) = delete;
		EditorRegistry& operator=(const EditorRegistry& other) = delete;
		EditorRegistry& operator=(EditorRegistry&& other) = delete;

	};
}
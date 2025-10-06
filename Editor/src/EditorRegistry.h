#pragma once

#include "UI/DetailsPainter.h"
#include "UI/ComponentPainter.h"

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <functional>
#include <type_traits>

namespace Twisted::Editor
{
	class DetailsPainter;
	class ComponentPainter;

	class EditorRegistry
	{
	public:
		EditorRegistry(const EditorRegistry& other) = delete;
		EditorRegistry(EditorRegistry&& other) = delete;
		EditorRegistry& operator=(const EditorRegistry& other) = delete;
		EditorRegistry& operator=(EditorRegistry&& other) = delete;

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
		void RegisterComponentPainter()
		{
			static_assert(std::is_base_of_v<ComponentPainter, T>, "T must derive from ComponentPainter");
			CompPainters.emplace_back<URef<T>>(std::make_unique<T>());
		}

		std::unordered_map<std::type_index, URef<DetailsPainter>> DetailsRenderers;
		std::vector<URef<ComponentPainter>> CompPainters;

	private:

		EditorRegistry() = default;
	};
}
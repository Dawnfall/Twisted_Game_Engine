#include <pch.h>
#include "EnTT/entt.hpp"
#include "SystemBase.h"
#include "Twisted/Game/AComponent.h"

#pragma once
namespace Twisted
{
	class EcsManager
	{
	public:

		/// <summary>
		/// Add Component on entity with given id
		/// </summary>
		/// <typeparam name="T">type of component</typeparam>
		/// <param name="id">entity id</param>
		/// <returns>reference to created entity or existing if already there</returns>
		template<typename T>
		T& AddComponent(entt::entity id)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "Component must be of type AComponent");
			return m_registry.emplace<T>(id, id);
		}

		//template <typename... ComponentTypes>
		//void AddComponents(entt::entity id)
		//{
		//	//TODO: static assert variadic pack	
		//	(m_registry.emplace<ComponentTypes>(id, id), ...);
		//}

		//template<typename Iterator, typename... ComponentTypes>
		//void AddComponents(Iterator begin, Iterator end)//(const std::vector <entt::entity>& entities)
		//{
		//	for (Iterator it = begin; it != end; it++)
		//		(m_registry.insert<ComponentTypes>(*it, *it), ...);
		//}

		//template<typename T>
		//void DestroyComponent(entt::entity id)
		//{
		//	m_registry.remove<T>(id); //or erase if we know
		//}

		////TODO: not sure if works
		//template <typename... ComponentTypes>
		//void RemoveComponents(entt::entity id)
		//{
		//	(m_registry.remove<ComponentTypes>(id), ...);
		//}

		/// <summary>
		/// Creates an empty entity
		/// </summary>
		/// <returns>id of created entity</returns>
		entt::entity CreateEntity()
		{
			return m_registry.create();
		}

		/// <summary>
		/// Creates x empty entities
		/// </summary>
		/// <param name="amount">number of entities to create</param>
		/// <returns>vector of ids of created entities</returns>
		std::vector<entt::entity> CreateEntities(int amount)
		{
			std::vector<entt::entity> res;
			for (int i = 0; i < amount; i++)
				res.push_back(CreateEntity());

			return res;
		}


		/// <summary>
		/// Clears the registry and destroys everything
		/// </summary>
		void Clear()
		{
			m_registry.clear();
		}

		/// <summary>
		/// Destroys an entity with all components on it
		/// </summary>
		/// <param name="id">id of entity to destroy</param>
		void DestroyEntity(entt::entity id)
		{
			m_registry.destroy(id);
		}

		/// <summary>
		/// Destroys all entities along with all components on them
		/// </summary>
		/// <typeparam name="Iterator">iterator type</typeparam>
		/// <param name="begin">begin iterator</param>
		/// <param name="end">end iterator</param>
		template<typename Iterator>
		void DestroyEntities(Iterator begin, Iterator end)
		{
			m_registry.destroy(begin, end);
		}

		/// <summary>
		/// Destroys all components of given type
		/// </summary>
		/// <typeparam name="T">component type to destroy</typeparam>
		template<typename T>
		void DestroyAllOfType()
		{
			m_registry.clear<T>();
		}

		/// <summary>
		/// returns component of given type on entity with provided id
		/// </summary>
		/// <typeparam name="T">component type</typeparam>
		/// <param name="id">entity id</param>
		/// <returns>pointer to component or nullptr if none</returns>
		template <typename T>
		T* GetComponent(entt::entity id)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.try_get<T>(id);
		}

		/// <summary>
		/// const variant of GetComponent
		/// </summary>
		/// <typeparam name="T">component type</typeparam>
		/// <param name="id">entity id</param>
		/// <returns>pointer to const component or nullptr if none</returns>
		template <typename T>
		const T* GetComponent(entt::entity id)const
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.try_get<T>(id);
		}

		/// <summary>
		/// returns pointer of first encountered component of given type
		/// </summary>
		/// <typeparam name="T">component type</typeparam>
		/// <returns>pointer of component or nullptr if none</returns>
		template<typename T>
		T* GetComponent()
		{
			static_assert(std::is_base_of<AComponent, T>::value);
			auto view = m_registry.view<T>();
			for (auto entity : view) {
				return &view.get<T>(entity);
			}
			return nullptr;
		}

		/// <summary>
		/// returns const pointer of first encountered component of given type
		/// </summary>
		/// <typeparam name="T">component type</typeparam>
		/// <returns>pointer of component or nullptr if none</returns>
		template<typename T>
		const T* GetComponent()const
		{
			static_assert(std::is_base_of<AComponent, T>::value);
			auto view = m_registry.view<T>();
			for (auto entity : view) {
				return &view.get<T>(entity);
			}
			return nullptr;
		}

		/// <summary>
		/// Returns a view of all components of type T in the registry
		/// </summary>
		/// <typeparam name="T">Component type</typeparam>
		/// <returns>entt::view of components of type T</returns>
		template<typename T>
		auto GetComponents()
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.view<T>();
		}

		/// <summary>
		/// Returns a const view of all components of type T in the registry
		/// </summary>
		/// <typeparam name="T">Component type</typeparam>
		/// <returns>entt::view of components of type T</returns>
		template<typename T>
		const auto GetComponents() const
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.view<T>();
		}

		template<typename T1,typename T2>
		const auto GetComponents() const
		{
			static_assert(std::is_base_of<AComponent, T1>::value, "T1 must derive from AComponent");
			static_assert(std::is_base_of<AComponent, T2>::value, "T2 must derive from AComponent");

			return m_registry.view<T1,T2>();
		}

		template<typename T1, typename T2>
		auto GetComponents()
		{
			static_assert(std::is_base_of<AComponent, T1>::value, "T1 must derive from AComponent");
			static_assert(std::is_base_of<AComponent, T2>::value, "T2 must derive from AComponent");

			return m_registry.view<T1, T2>();
		}

		//*******************
		// Systems
		void UpdateSystems(Application* app)
		{
			for (auto& system : m_systems)
				system->Update(app);
		}

		template<typename T>
		void AddSystem()
		{
			static_assert(std::is_base_of <SystemBase, T>::value, "T must derive from SystemBase");
			m_systems.push_back(std::make_unique<T>());
		}



		//entt::registry& GetRegistry() { return m_registry; }
	private:
		entt::registry m_registry;
		std::vector<std::unique_ptr<SystemBase>> m_systems;
	};
}
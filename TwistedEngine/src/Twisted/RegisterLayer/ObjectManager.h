
#pragma once
#include "AppCore.h"
#include "BaseObject.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;
namespace Twisted
{
	class TWISTED_API ObjectManager
	{
	public:

		static ObjectManager& GetInstance()
		{
			static ObjectManager instance;
			return instance;
		}

		ObjectManager(const ObjectManager&) = delete;
		ObjectManager& operator=(const ObjectManager&) = delete;
		ObjectManager(ObjectManager&&) = delete;
		ObjectManager& operator=(ObjectManager&&) = delete;

		/// <summary>
		/// creates a new object derived from BaseObject
		/// </summary>
		/// <typeparam name="T">type of object to create(must derive from BaseObject)</typeparam>
		/// <typeparam name="...Args">args for object without id</typeparam>
		/// <param name="...args">args for object without id</param>
		/// <returns>newly created object</returns>
		template<typename T, typename... Args>
		T* CreateObject(Args&&... args)
		{
			static_assert(std::is_base_of_v<BaseObject, T>, "T must derive from BaseObject");

			ObjectID id = CreateID();
			URef<T> newObj = std::make_unique<T>(id, std::forward<Args>(args)...);
			T* objPtr = newObj.get();
			m_objects[id.Index()] = std::move(newObj);
			return objPtr;
		}

		/// <summary>
		/// reload existing object using the same ID, assumes id is valid
		/// </summary>
		/// <typeparam name="T">type of object to create(must derive from BaseObject)</typeparam>
		/// <typeparam name="...Args">args for object without id</typeparam>
		/// <param name="obj">id of existing object</param>
		/// <param name="...args">args for object without id</param>
		/// <returns>newly created object</returns>
		template<typename T, typename... Args>
		T* ReloadObject(ObjectID obj, Args&& ...args)
		{
			static_assert(std::is_base_of_v<BaseObject, T>, "T must derive from BaseObject");

			URef<T> newObj = std::make_unique<T>(obj, std::forward<Args>(args)...);
			T* newPtr = newObj.get();
			m_objects[obj.Index()] = std::move(newObj);
			return newPtr;
		}

		/// <summary>
		/// object referred by id , nullptr if invalid
		/// </summary>
		/// <typeparam name="T">type of object to create (must derive from BaseObject)</typeparam>
		/// <param name="id">id of object</param>
		/// <returns>object with provided id or nullptr if invalid</returns>
		template<typename T>
		T* TryGetObject(ObjectID id)
		{
			if (id.Index() < m_generation.size() &&
				m_generation[id.Index()] == id.Serial() &&
				m_objects[id.Index()])
			{
				return static_cast<T*>(m_objects[id.Index()].get());
			}
			return nullptr;
		}

		/// <summary>
		/// object referred by id , nullptr if invalid (const alternative)
		/// </summary>
		/// <typeparam name="T">type of object to create (must derive from BaseObject)</typeparam>
		/// <param name="id">id of object</param>
		/// <returns>object with provided id or nullptr if invalid</returns>
		template<typename T>
		const T* TryGetObject(ObjectID id) const
		{
			if (id.Index() < m_generation.size() &&
				m_generation[id.Index()] == id.Serial() &&
				m_objects[id.Index()])
			{
				return static_cast<const T*>(m_objects[id.Index()].get());
			}
			return nullptr;
		}

		/// <summary>
		/// object referred by id , assumes valid (throws if invalid)
		/// </summary>
		/// <typeparam name="T">type of object to create (must derive from BaseObject)</typeparam>
		/// <param name="id">id of object</param>
		/// <returns>object with provided id</returns>
		template<typename T>
		T* GetIdObject(ObjectID id)
		{
			T* obj = TryGetObject<T>(id);
			if (!obj) {
				throw std::runtime_error("Invalid ObjectID");
			}
			return obj;
		}

		/// <summary>
		/// object referred by id , assumes valid (throws if invalid) (const alternative)
		/// </summary>
		/// <typeparam name="T">type of object to create (must derive from BaseObject)</typeparam>
		/// <param name="id">id of object</param>
		/// <returns>object with provided id</returns>
		template<typename T>
		const T* GetIdObject(ObjectID id) const
		{
			const T* obj = TryGetObject<T>(id);
			if (!obj) {
				throw std::runtime_error("Invalid ObjectID");
			}
			return obj;
		}

		/// <summary>
		/// destroys object with given id, recycles id
		/// </summary>
		/// <param name="id">id of object to destroy</param>
		void DestroyID(ObjectID id)
		{
			if (id.Index() < m_generation.size() && m_generation[id.Index()] == id.Serial())
			{
				m_objects[id.Index()].reset();
				freeIndices.push_back(id.Index());
			}
		}

	private:

		ObjectManager() = default;

		/// <summary>
		/// created new valid ID
		/// </summary>
		/// <returns>newly created id</returns>
		ObjectID CreateID()
		{
			uint32_t idx;
			if (!freeIndices.empty())
			{
				idx = freeIndices.front();
				freeIndices.pop_back();
				m_generation[idx]++;
			}
			else
			{
				idx = static_cast<uint32_t>(m_generation.size());

				if (idx >= m_objects.size())
				{
					size_t newSize = std::max<size_t>(m_objects.size() * 2, idx + 1);
					m_objects.resize(newSize);
					m_generation.resize(newSize);
				}
			}

			return ObjectID(idx, m_generation[idx]);
		}

		std::vector<URef<BaseObject>> m_objects;
		std::vector<uint32_t> m_generation;
		std::vector<uint32_t> freeIndices;
	};
}
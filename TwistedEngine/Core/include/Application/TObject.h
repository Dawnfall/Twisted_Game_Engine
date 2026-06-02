#pragma once

#include "AppCore.h"
#include "ObjectID.h"
#include <memory>
#include <vector>

namespace Twisted
{
	class TWISTED_API TObject
	{
	public:
		TObject(const std::string& name) :
			m_name(name)
		{
		}

		virtual ~TObject() = default;

		// No copy
		TObject(const TObject&) = delete;
		TObject& operator=(const TObject&) = delete;

		// Allow move
		TObject(TObject&&) noexcept = delete;
		TObject& operator=(TObject&&) noexcept = delete;

		ObjectID GetID() const { return m_id; }

		template<typename T>
		T* dynamic_as() { return dynamic_cast<T*>(this); }

		template<typename T>
		T* static_as() { return static_cast<T*>(this); }

		void SetName(const std::string& name) { m_name = name; }
		const std::string& GetName()const { return m_name; }

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
	private:
		ObjectID    m_id;
		std::string m_name = "";


		//STATIC

	public:
		template<typename T, typename... Args>
		inline static T* Create(Args&&... args)
		{
			static_assert(std::is_base_of_v<TObject, T>, "T must derive from TObject");

			ObjectID id = AllocateID();
			uint32_t index = id.Index();

			s_objects[index] = std::make_unique<T>(std::forward<Args>(args)...);
			s_objects[index]->m_id = id;
			s_objects[index]->OnCreate();

			return static_cast<T*>(s_objects[index].get());
		}

		inline static void Destroy(TObject* obj)
		{
			if (!obj)
				return;

			ObjectID id = obj->GetID();
			obj->OnDestroy();
			DestroyID(id);
		}

		inline static void DestroyAll()
		{
			for (auto& obj : s_objects)
				if (obj)
					Destroy(obj.get());
			s_objects.clear();
			s_freeIndices.clear();
			s_generation.clear();
		}

		inline static TObject* GetObj(ObjectID id)
		{
			if (IsValidID(id))
				return s_objects[id.Index()].get();
			return nullptr;
		}

		inline static bool IsValidID(ObjectID id)
		{
			uint32_t index = id.Index();
			return index < s_generation.size() && s_generation[index] == id.Generation();
		}

		static void Initialize();

	private:
		static ObjectID AllocateID()
		{
			uint32_t index;
			if (!s_freeIndices.empty())
			{
				index = s_freeIndices.back();
				s_freeIndices.pop_back();
				s_generation[index]++;
			}
			else
			{
				index = static_cast<uint32_t>(s_generation.size());
				s_generation.emplace_back(1);
				s_objects.emplace_back(nullptr);
			}

			return ObjectID(index, s_generation[index]);
		}
		static void DestroyID(ObjectID id)
		{
			uint32_t index = id.Index();
			if (index >= s_generation.size() || s_generation[index] != id.Generation())
				throw std::runtime_error("Destroying non existing index");

			s_objects[index].reset();
			s_generation[index]++;
			s_freeIndices.push_back(index);
		}


	private:
		static std::vector<URef<TObject>> s_objects;
		static std::vector<uint32_t> s_generation;
		static std::vector<uint32_t> s_freeIndices;
	};
}




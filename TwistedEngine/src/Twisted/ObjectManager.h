#pragma once

#include "AppCore.h"
#include "TObject.h"
#include "Debug/Logger.h"
namespace Twisted
{
	class TWISTED_API ObjectManager
	{
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
		}

		inline static TObject* GetObj(ObjectID id)
		{
			if (id.IsValid())
				return s_objects[id.Index()].get();
			return nullptr;
		}

		inline static bool IsValidID(ObjectID id)
		{
			uint32_t index = id.Index();
			return index < s_generation.size() && s_generation[index] == id.Generation();
		}

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

		friend class TObject;
		friend class ObjectID;
	};
}
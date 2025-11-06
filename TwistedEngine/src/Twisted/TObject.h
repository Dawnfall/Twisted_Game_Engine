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
		virtual ~TObject()
		{
			ObjectID::DestroyID(GetID());
		}

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

	protected:
		TObject(const std::string& name) :m_id(ObjectID::AllocateID()),m_name(name) {}
	private:
		ObjectID m_id;
		std::string m_name = "";

	public:
		// Create a new TObject of type T
		template<typename T, typename... Args>
		inline static T* Create(Args&&... args)
		{
			static_assert(std::is_base_of_v<TObject, T>, "T must derive from TObject");

			auto obj = std::make_unique<T>(std::forward<Args>(args)...);

			T* ptr = obj.get();

			EnsureStorageSize(obj->GetID().Index());
			s_objects[obj->GetID().Index()] = std::move(obj);

			return ptr;
		}

		// Destroy object by pointer
		inline static void Destroy(TObject* obj)
		{
			if (!obj)
				return;

			s_objects[obj->GetID().Index()] = nullptr;			
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
	private:

		inline static std::vector<URef<TObject>> s_objects;

	private:
		static void EnsureStorageSize(size_t index)
		{
			auto size = s_objects.size();
			if (index >= size)
				s_objects.resize((size == 0) ? 1 : size * 2);
		}
	};
}


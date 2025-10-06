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
		TObject():m_id(ObjectID::AllocateID()){}
		virtual ~TObject() = default;

		// No copy
		TObject(const TObject&) = delete;
		TObject& operator=(const TObject&) = delete;

		// Allow move
		TObject(TObject&&) noexcept = default;
		TObject& operator=(TObject&&) noexcept = default;

		ObjectID GetID() const { return m_id; }

		template<typename T>
		T* dynamic_as() { return dynamic_cast<T*>(this); }

		template<typename T>
		T* static_as() { return static_cast<T*>(this); }

	private:
        ObjectID m_id;

	public:
		// Create a new TObject of type T
		template<typename T, typename... Args>
		static T* Create(Args&&... args)
		{
			static_assert(std::is_base_of_v<TObject, T>, "T must derive from TObject");

			auto obj = std::make_unique<T>(std::forward<Args>(args)...);

			T* ptr = obj.get();

			EnsureStorageSize(obj->GetID().Index());
			s_objects[obj->GetID().Index()] = std::move(obj);

			return ptr;
		}

		// Destroy object by pointer
		static void Destroy(TObject* obj)
		{
			if (!obj)
				return;

			s_objects[obj->GetID().Index()].reset();
			ObjectID::DestroyID(obj->GetID());
		}
	private:

		inline static std::vector<URef<TObject>> s_objects;

	private:
		static void EnsureStorageSize(size_t index) //TODO....
		{
			if (index >= s_objects.size())
				s_objects.resize(index + 1);
		}
	};
}
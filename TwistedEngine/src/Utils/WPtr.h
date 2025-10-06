#pragma once

#include "AppCore.h"
#include <cstdint>
#include <vector>
#include <memory>
#pragma once

#include "WPtrBase.h"
#include <type_traits>

namespace Twisted
{
	template<typename T>
	class WPtr : public WPtrBase
	{
		static_assert(std::is_base_of_v<TObject, T>, "T must derive from TObject");
	public:
		WPtr() {}
		WPtr(std::nullptr_t) noexcept :WPtr() { }
		explicit WPtr(T* ptr) : WPtrBase(ptr) {  }

		WPtr<T>& operator=(T* ptr)
		{
			if (ptr)
				m_id = ptr->GetID();
			else
				m_id = ObjectID(); // invalid
			return *this; // return reference to self
		}
		WPtr& operator=(std::nullptr_t) noexcept
		{
			m_id = ObjectID(); // make invalid
			return *this;
		}

		// Access the object
		[[nodiscard]] T* get() noexcept { return static_cast<T*>(GetObj()); }
		[[nodiscard]] const T* get() const noexcept { ;return static_cast<const T*>(GetObj()); }

		[[nodiscard]] T& operator*() noexcept { return *get(); }
		[[nodiscard]] const T& operator*() const noexcept { return *get(); }

		[[nodiscard]] T* operator->() noexcept { return get(); }
		[[nodiscard]] const T* operator->() const noexcept { return get(); }

		explicit operator bool() const noexcept { return get() != nullptr; }
	};
}
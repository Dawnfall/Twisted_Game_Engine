#pragma once

#include "AppCore.h"
#include "Twisted/TObject.h"
#include <cstddef>
namespace Twisted
{
	class TWISTED_API WPtrBase
	{
	public:
		WPtrBase() = default;
		WPtrBase(std::nullptr_t) noexcept :WPtrBase() {}
		explicit WPtrBase(TObject* ptr) : m_ptr(ptr), m_id(ptr ? ptr->GetID() : ObjectID(0)) {}

		ObjectID GetID() const { return m_id; }
		TObject* GetObj() { return Validate() ? m_ptr : nullptr; }
		const TObject* GetObj()const { return Validate() ? m_ptr : nullptr; }

		// Comparison operators
		friend bool operator==(const WPtrBase& a, const WPtrBase& b) { return a.GetID() == b.GetID(); }
		friend bool operator!=(const WPtrBase& a, const WPtrBase& b) { return a.GetID() != b.GetID(); }

	protected:

		bool Validate() const noexcept
		{
			return m_ptr && m_id.IsValid();
		}

	protected:
		TObject* m_ptr = nullptr;
		ObjectID m_id;
	};
}

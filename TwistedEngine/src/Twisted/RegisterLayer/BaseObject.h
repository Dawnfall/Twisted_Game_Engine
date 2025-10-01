#pragma once

#include "ObjectID.h"

namespace Twisted
{
	class BaseObject
	{
	public:
		explicit BaseObject(ObjectID id) : m_id(id) {}
		virtual ~BaseObject() = default;

		ObjectID getID() const noexcept { return m_id; }

		BaseObject(const BaseObject&) = delete;
		BaseObject& operator=(const BaseObject&) = delete;
		BaseObject(BaseObject&&) noexcept = default;
		BaseObject& operator=(BaseObject&&) noexcept = default;
	private:
		ObjectID m_id;
	};
}
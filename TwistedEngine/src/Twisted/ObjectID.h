#pragma once

#include "AppCore.h"
#include <cstdint>
#include <vector>
#include <string>

namespace Twisted
{
	static constexpr int INDEX_BITS = 32;

	class TWISTED_API ObjectID
	{
	public:
		ObjectID() = default;
		explicit ObjectID(uint64_t id) : m_id(id) {}
		ObjectID(uint32_t index, uint32_t gen) : m_id((uint64_t(gen) << INDEX_BITS) | index) {}

		uint64_t GetID() const { return m_id; }
		uint32_t Index() const { return uint32_t(m_id & ((1ULL << INDEX_BITS) - 1)); }
		uint32_t Generation() const { return uint32_t(m_id >> INDEX_BITS); }

		bool IsValid() const;
		std::string ToString()const;

		bool operator==(const ObjectID& other) const { return m_id == other.m_id; }
		bool operator!=(const ObjectID& other) const { return m_id != other.m_id; }

	private:
		uint64_t m_id = 0;
	};
}

template<>
struct std::hash<Twisted::ObjectID>
{
	size_t operator()(const Twisted::ObjectID& e) const noexcept
	{
		return static_cast<size_t>(e.GetID());
	}
};


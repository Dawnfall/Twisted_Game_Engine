#pragma once

#include "AppCore.h"
#include <cstdint>
#include <vector>
#include <string>

namespace Twisted
{
	static constexpr int INDEX_BITS = 32;
	static constexpr int SERIAL_BITS = 32;

	class TWISTED_API ObjectID
	{
	public:
		ObjectID() = default;
		explicit ObjectID(uint64_t id) : m_id(id) {}
		ObjectID(uint32_t index, uint32_t gen) : m_id((uint64_t(gen) << INDEX_BITS) | index) {}

		uint64_t GetID() const { return m_id; }
		uint32_t Index() const { return uint32_t(m_id & ((1ULL << INDEX_BITS) - 1)); }
		uint32_t Generation() const { return uint32_t(m_id >> INDEX_BITS); }

		bool IsValid() const
		{
			uint32_t index = Index();
			return index < s_generation.size() && s_generation[index] == Generation();
		}

		// Allocate a new ID
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
				s_generation.resize(index + 1);
			}

			uint32_t gen = s_generation[index];
			return ObjectID(index, gen);
		}

		// Invalidate an ID
		static void DestroyID(const ObjectID& id)
		{
			uint32_t index = id.Index();
			if (index >= s_generation.size()) 
				return;
			s_generation[index]++;
			s_freeIndices.push_back(index);
		}

		std::string ToString()const
		{
			return std::to_string(m_id);
		}

		bool operator==(const ObjectID& other) const { return m_id == other.m_id; }
		bool operator!=(const ObjectID& other) const { return m_id != other.m_id; }

	private:
		uint64_t m_id = 0;

		inline static std::vector<uint32_t> s_generation;
		inline static std::vector<uint32_t> s_freeIndices;
	};
}


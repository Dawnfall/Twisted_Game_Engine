#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>

namespace Twisted
{
	struct AssetUuid
	{
		constexpr AssetUuid() : m_hi(0), m_lo(0) {}
		constexpr AssetUuid(uint64_t high, uint64_t low) : m_hi(high), m_lo(low) {}

		bool IsValid() const { return m_hi != 0 || m_lo != 0; }

		std::string ToString() const
		{
			std::ostringstream oss;
			oss << std::hex << std::setfill('0')
				<< std::setw(16) << m_hi
				<< std::setw(16) << m_lo;
			return oss.str();
		}

		static AssetUuid FromString(const std::string& s)
		{
			if (s.size() != 32)
				return Invalid();
			uint64_t hi = 0, lo = 0;
			std::istringstream hiStream(s.substr(0, 16));
			std::istringstream loStream(s.substr(16, 16));
			if (!(hiStream >> std::hex >> hi) || !(loStream >> std::hex >> lo))
				return Invalid();
			return AssetUuid{ hi, lo };
		}

		bool operator==(const AssetUuid& other) const noexcept { return m_hi == other.m_hi && m_lo == other.m_lo; }
		bool operator!=(const AssetUuid& other) const noexcept { return !(*this == other); }

		// Defined in AssetUuid.cpp — requires platform UUID generation
		static AssetUuid generate();

		static const AssetUuid& Invalid()
		{
			static AssetUuid invalidUuid;
			return invalidUuid;
		}

	private:
		uint64_t m_hi;
		uint64_t m_lo;

		friend struct std::hash<AssetUuid>;
	};
}

namespace std
{
	template<>
	struct hash<Twisted::AssetUuid>
	{
		size_t operator()(const Twisted::AssetUuid& u) const noexcept
		{
			size_t h = std::hash<uint64_t>()(u.m_hi);
			h ^= std::hash<uint64_t>()(u.m_lo) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
			return h;
		}
	};
}

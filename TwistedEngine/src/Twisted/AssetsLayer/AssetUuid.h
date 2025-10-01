#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>
#include <objbase.h> 
#include <iomanip>
#include <sstream>
#include <yaml-cpp/yaml.h>
#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib") //TODO: replace with... target_link_libraries(MyApp PRIVATE Rpcrt4.lib)

namespace Twisted
{
	struct AssetUuid
	{
		AssetUuid() : m_hi(0), m_lo(0) {}
		AssetUuid(uint64_t high, uint64_t low) : m_hi(high), m_lo(low) {}

		bool IsValid()const { return m_hi != 0 && m_lo != 0; }
		std::string ToString() const //this is without dashes, maybe change in future
		{
			std::ostringstream oss;
			oss << std::hex << std::setfill('0')
				<< std::setw(16) << m_hi
				<< std::setw(16) << m_lo;

			return oss.str();
		}


		bool operator==(const AssetUuid& other) const noexcept
		{
			return m_hi == other.m_hi && m_lo == other.m_lo;
		}
		bool operator!=(const AssetUuid& other) const noexcept
		{
			return !(*this == other);
		}

		static AssetUuid generate()
		{
			GUID g;
			if (UuidCreate(&g) != RPC_S_OK)
				throw std::runtime_error("Failed to generate UUID");

			// GUID is 16 bytes: Data1(4), Data2(2), Data3(2), Data4(8)
			uint64_t hi =
				(static_cast<uint64_t>(g.Data1) << 32) |
				(static_cast<uint64_t>(g.Data2) << 16) |
				static_cast<uint64_t>(g.Data3);
			uint64_t lo = 0;
			for (int i = 0; i < 8; ++i)
				lo = (lo << 8) | g.Data4[i];

			return AssetUuid{ hi, lo };


			//GUID guid;
			//if (CoCreateGuid(&guid) == S_OK)
			//{
			//	char guidString[39]; // 38 characters + null terminator
			//	snprintf(guidString, sizeof(guidString),
			//		"{%08x-%04x-%04x-%04x-%012llx}",
			//		guid.Data1, guid.Data2, guid.Data3,
			//		(guid.Data4[0] << 8) | guid.Data4[1],
			//		*((unsigned long long*) & guid.Data4[2]));
			//	return std::string(guidString);
			//}
			//TWISTED_ERROR("Failed to create GUID");
			//return "";
		}

		static const AssetUuid& Invalid()
		{
			static AssetUuid invalidUuid;
			return invalidUuid;
		}

	private:
		uint64_t m_hi;
		uint64_t m_lo;

		friend std::hash<AssetUuid>;
		friend YAML::convert<AssetUuid>;
	};
}

namespace std {
	template <>
	struct hash<Twisted::AssetUuid>
	{
		size_t operator()(const Twisted::AssetUuid& u) const noexcept
		{
			uint64_t x = u.m_hi ^ (u.m_lo + 0x9e3779b97f4a7c15ULL + (u.m_hi << 6) + (u.m_lo >> 2));
			return std::hash<uint64_t>()(x);
		}
	};
}

template<>
struct YAML::convert<Twisted::AssetUuid>
{
	static YAML::Node encode(const Twisted::AssetUuid id)
	{
		Node node;
		node = id.ToString();
		return node;
	}

	static bool decode(const YAML::Node& node, Twisted::AssetUuid& uuid)
	{
		if (!node.IsScalar())
			return false;

		std::string s = node.Scalar();
		if (s.size() != 32)
			return false;

		uint64_t hi, lo;
		std::istringstream hiStream(s.substr(0, 16));
		std::istringstream loStream(s.substr(16, 16));

		if (!(hiStream >> std::hex >> hi) || !(loStream >> std::hex >> lo))
			return false;

		uuid = Twisted::AssetUuid{ hi, lo };
		return true;
	}
};
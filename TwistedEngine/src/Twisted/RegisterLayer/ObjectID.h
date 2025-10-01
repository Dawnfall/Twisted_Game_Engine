#pragma once

#include <cstdint>
#include <functional>
#include <yaml-cpp/yaml.h>

namespace Twisted
{
	static constexpr int INDEX_BITS = 32;
	static constexpr int SERIAL_BITS = 32;

	class ObjectID
	{
	public:
		ObjectID() : m_id(0) {}
		explicit ObjectID(uint64_t id) :m_id(id) {}

		ObjectID(uint32_t index, uint32_t serial)
		{
			m_id = (uint64_t(serial) << INDEX_BITS) | uint64_t(index);
		}
		uint32_t Index() const { return uint32_t(m_id & ((1ULL << INDEX_BITS) - 1)); }
		uint32_t Serial() const { return uint32_t((m_id >> INDEX_BITS) & ((1ULL << SERIAL_BITS) - 1)); }
		uint64_t GetID()const { return m_id; }
		bool IsValid() const { return m_id != 0; }

		explicit operator bool() const noexcept { return *this != ObjectID::Invalid(); }

		static const ObjectID& Invalid()
		{
			static ObjectID invalidID;
			return invalidID;
		}

		friend bool operator==(ObjectID a, ObjectID b) { return a.m_id == b.m_id; }
		friend bool operator!=(ObjectID a, ObjectID b) { return a.m_id != b.m_id; }
		

	private:
		uint64_t m_id;
	};
}

namespace std
{
	template <>
	struct hash<Twisted::ObjectID>
	{
		size_t operator()(const Twisted::ObjectID& id) const
		{
			return hash<uint64_t>()(id.GetID());
		}
	};
}

//TODO: probably remove since not intended for serialization
//template<>
//struct YAML::convert<Twisted::ObjectID> 
//{
//	static YAML::Node encode(const Twisted::ObjectID id)
//	{
//		// Store as a single scalar string: "UUID:subID"
//		return YAML::Node(id.GetID());
//	}
//
//	static bool decode(const YAML::Node& node, Twisted::ObjectID& id) {
//		if (!node.IsScalar())
//			return false;
//		uint64_t idValue = node.as<uint64_t>();
//		id = Twisted::ObjectID(idValue);
//		return true;
//	}
//};
#pragma once
#include "AssetUuid.h"
#include "Twisted/RegisterLayer/ObjectID.h"

#include <string>
#include "Twisted/RegisterLayer/Serialization/BinSerializer.h"
#include "Utils/WPtr.h"

namespace Twisted
{
	struct ObjectAssetEntry
	{
		AssetUuid Uuid;
		std::string SubID;

		static const ObjectAssetEntry& Invalid()
		{
			static ObjectAssetEntry invalidEntry;
			return invalidEntry;
		}

		bool operator==(const ObjectAssetEntry& other)const
		{
			return
				this->Uuid == other.Uuid &&
				this->SubID == other.SubID;
		}
		bool operator!=(const ObjectAssetEntry& other)const { return !((*this) == other); }

		explicit operator bool() const noexcept
		{
			return (*this) != ObjectAssetEntry::Invalid();
		}
	};

	inline void writeToBuffer(const ObjectAssetEntry& obj, BinSerializer& buf)
	{
		buf.Write(obj.Uuid);
		buf.Write(obj.SubID);
	}

	inline ObjectAssetEntry readFromBuffer(BinSerializer& buf)
	{
		ObjectAssetEntry objAssetEntry;
		objAssetEntry.Uuid = buf.Read<AssetUuid>();
		objAssetEntry.SubID = buf.Read<std::string>();

		return objAssetEntry;
	}
}

template<>
struct YAML::convert<Twisted::ObjectAssetEntry>
{
	inline static YAML::Node encode(const Twisted::ObjectAssetEntry id)
	{
		Node node;
		node["uuid"] = id.Uuid;
		node["subID"] = id.SubID;
		return node;
	}

	inline static bool decode(const YAML::Node& node, Twisted::ObjectAssetEntry& id)
	{
		Twisted::AssetUuid uuid = node["uuid"].as<Twisted::AssetUuid>();
		std::string subID = node["subID"].as<std::string>();

		id = { uuid,subID };
		return true;
	}
};
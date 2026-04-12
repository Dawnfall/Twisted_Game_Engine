#include "Twisted/AssetsLayer/AssetUuid.h"

#include <objbase.h>
#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

namespace Twisted
{
	AssetUuid AssetUuid::generate()
	{
		GUID g;
		if (UuidCreate(&g) != RPC_S_OK)
			throw std::runtime_error("Failed to generate UUID");

		uint64_t hi =
			(static_cast<uint64_t>(g.Data1) << 32) |
			(static_cast<uint64_t>(g.Data2) << 16) |
			static_cast<uint64_t>(g.Data3);

		uint64_t lo = 0;
		for (int i = 0; i < 8; ++i)
			lo = (lo << 8) | g.Data4[i];

		return AssetUuid{ hi, lo };
	}
}

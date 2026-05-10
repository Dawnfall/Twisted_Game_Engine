#include "AssetUuid.h"

#include <random>

namespace Twisted
{
	AssetUuid AssetUuid::generate()
	{
		static std::mt19937_64 gen{ std::random_device{}() };
		static std::uniform_int_distribution<uint64_t> dist;

		uint64_t hi = dist(gen);
		uint64_t lo = dist(gen);

		// RFC 4122 v4: version bits
		hi = (hi & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL;
		// variant bits
		lo = (lo & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL;

		return AssetUuid{ hi, lo };
	}
}

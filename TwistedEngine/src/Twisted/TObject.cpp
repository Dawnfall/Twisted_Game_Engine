#include "Twisted/TObject.h"
#include <cstdint>
#include <AppCore.h>
#include <vector>

namespace Twisted
{
	constexpr size_t startCapacity = 512;

	void TObject::Initialize()
	{
		s_objects.reserve(startCapacity);
		s_generation.reserve(startCapacity);
		s_freeIndices.reserve(startCapacity);
	}

	struct ObjectManagerInitializer
	{
		ObjectManagerInitializer() { TObject::Initialize(); }
	} s_init; // Runs before main()
}


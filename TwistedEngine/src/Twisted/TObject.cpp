#include "Twisted/TObject.h"
#include <cstdint>
#include <AppCore.h>
#include <vector>

namespace Twisted
{
	constexpr size_t startCapacity = 512;

	std::vector<URef<TObject>> TObject::s_objects;
	std::vector<uint32_t> TObject::s_generation;
	std::vector<uint32_t> TObject::s_freeIndices;



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


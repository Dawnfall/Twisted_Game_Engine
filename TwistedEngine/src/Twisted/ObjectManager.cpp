#include "ObjectManager.h"

namespace Twisted
{
	std::vector<URef<TObject>> ObjectManager::s_objects;
	std::vector<uint32_t> ObjectManager::s_generation;
	std::vector<uint32_t> ObjectManager::s_freeIndices;
}
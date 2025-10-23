#include "AssetImporterRegistry.h"

namespace Twisted
{
	AssetImporterRegistry& AssetImporterRegistry::GetInstance()
	{
		static AssetImporterRegistry instance;
		return instance;
	}


}
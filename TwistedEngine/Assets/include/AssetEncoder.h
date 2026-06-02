#pragma once

#include "AppCore.h"
#include <yaml-cpp/yaml.h>

namespace Twisted
{
	class TObject;

	TWISTED_API YAML::Node encodeTObject(const Twisted::TObject* obj);
	TWISTED_API Twisted::TObject* decodeTObject(const YAML::Node& node);
}

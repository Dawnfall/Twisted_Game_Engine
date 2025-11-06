#include "SerializedBuffer.h"

#include "Utils/YamlUtils.h"

namespace Twisted
{
	class YamlSerializer :SerializedBuffer
	{
	public:
		YAML::Node Data;
	private:
	};

}
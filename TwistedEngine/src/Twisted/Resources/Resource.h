#pragma once

#include <string>

namespace Twisted
{
	struct Resource
	{
		virtual ~Resource() {}

		std::string Name = "";
	};
}
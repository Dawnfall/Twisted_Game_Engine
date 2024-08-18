#pragma once

#include <string>

namespace Dawn
{
	struct Resource
	{
		virtual ~Resource() {}

		std::string Name = "";
	};
}
#pragma once

#include <string>

namespace Dawn
{
	struct Resource
	{
		Resource(const std::string& name) :Name(name) {}
		virtual ~Resource() {}

		std::string Name = "";
	};
}
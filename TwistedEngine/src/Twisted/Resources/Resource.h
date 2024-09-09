#pragma once

#include "pch.h"

namespace Twisted
{
	struct Resource
	{
		virtual ~Resource() {}

		std::string Name = "";
	};
}
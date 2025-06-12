#pragma once

#include "AppCore.h"
#include "Utils/Utils.h"
#include <string>

namespace Twisted
{
	class TWISTED_API UUIDObject
	{
	public:
		UUIDObject() :m_uuid(Utils::GenerateGUID()) {}

		const std::string& GetUUID()const { return m_uuid; }
	protected:
		std::string m_uuid;
	};
}
#include "editorpch.h"
#include "EditorUtils.h"
#include "Debug/Logger.h"

namespace Twisted::Editor
{
	std::string GenerateGUID()
	{
		GUID guid;
		if (CoCreateGuid(&guid) == S_OK)
		{
			char guidString[39]; // 38 characters + null terminator
			snprintf(guidString, sizeof(guidString),
				"{%08x-%04x-%04x-%04x-%012llx}",
				guid.Data1, guid.Data2, guid.Data3,
				(guid.Data4[0] << 8) | guid.Data4[1],
				*((unsigned long long*) & guid.Data4[2]));
			return std::string(guidString);
		}
		TWISTED_ERROR("Failed to create GUID");
		return "";
	}
}
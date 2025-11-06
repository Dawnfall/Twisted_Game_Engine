#pragma once

#include <filesystem>
namespace Twisted
{
	class SerializedBuffer
	{
		virtual bool SaveToFile(const std::filesystem::path& path) = 0;
		virtual bool LoadFromFile(const std::filesystem::path& path) = 0;
	};
}
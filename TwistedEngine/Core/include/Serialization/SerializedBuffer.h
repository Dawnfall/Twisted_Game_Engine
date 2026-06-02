#pragma once

#include <filesystem>
namespace Twisted
{
	class SerializedBuffer
	{
	public:
		virtual ~SerializedBuffer() = default;
		virtual bool SaveToFile(const std::filesystem::path& path) = 0;
		virtual bool LoadFromFile(const std::filesystem::path& path) = 0;
	};
}


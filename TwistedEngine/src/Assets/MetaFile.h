#pragma once

namespace Twisted::Editor
{
	class MetaFile
	{
	public:
		bool Read(const std::string& metaFile);
		bool Write(const std::string& metaFile);

		std::string Version = "1.0_TODO";
		std::string Type="";
		std::string AssetPath="";
		std::string AssetName="";
		std::string Uuid="";
	};
}
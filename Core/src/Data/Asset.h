#pragma once
#include "Utils/Utils.h"

#include <filesystem>
#include <windows.h>
#include <string>

namespace Twisted
{
	struct Asset
	{
		Asset()
		{
			UUID = Utils::GenerateGUID();
		}
		virtual ~Asset()
		{

		}

		std::string UUID;
		std::filesystem::path FilePath;

		//bool HasMetaFile(){}
		//void LoadMetaFile(){}
		//void SaveMetaFile(){}
	};



}
#pragma once
#include "twistedpch.h"
#include "Utils/Utils.h"

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
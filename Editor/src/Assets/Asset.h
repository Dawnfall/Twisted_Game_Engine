#pragma once
#include "editorpch.h"
#include "Utils/EditorUtils.h"

namespace Twisted::Editor
{
	class Asset
	{
		std::string UUID;
		std::string filePath;

		Asset()
		{
			UUID = GenerateGUID();
		}

		bool HasMetaFile(){}
		void LoadMetaFile(){}
		void SaveMetaFile(){}

	};



}
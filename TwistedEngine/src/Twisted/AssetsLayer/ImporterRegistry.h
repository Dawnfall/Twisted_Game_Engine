#pragma once

#include "AppCore.h"
#include "AssetImporter.h"
#include <unordered_map>

namespace Twisted
{
	class ImporterRegistry
	{
	public:
		ImporterRegistry(const ImporterRegistry& other) = delete;
		ImporterRegistry(ImporterRegistry&& other) = delete;
		ImporterRegistry& operator=(const ImporterRegistry& other) = delete;
		ImporterRegistry& operator=(ImporterRegistry&& other) = delete;

		static ImporterRegistry& GetInstance()
		{
			static ImporterRegistry instance;
			return instance;
		}

		template<typename T>
		void RegisterImporter()
		{
			static_assert(std::is_base_of_v<AssetImporter, T>, "T must derive from AssetImporter");

			auto importer = std::make_shared<T>();

			for (const fs::path& ext : importer->GetAssetExtensions())
				m_extToImporter[ext] = importer;
		}

		AssetImporter* GetImporter(const fs::path& extension)
		{
			auto it = m_extToImporter.find(extension);
			if (it != m_extToImporter.end())
				return it->second.get();
			return nullptr;
		}

	private:
		ImporterRegistry() = default;
		std::unordered_map<fs::path, SRef<AssetImporter>> m_extToImporter; //TODO: maybe switch to string for ext
	};
}
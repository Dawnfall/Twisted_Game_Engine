#pragma once

#include "AppCore.h"

#include "AssetImporter.h"

#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

namespace Twisted
{
	class TWISTED_API AssetImporterRegistry
	{
	public:
		static AssetImporterRegistry& GetInstance();

		template<typename T>
		void RegisterImporter()
		{
			static_assert(std::is_base_of_v<AssetImporter, T>, "T must derive from AssetImporter");

			auto importer = std::make_shared<T>();

			for (const fs::path& ext : importer->GetAssetExtensions())
				m_extToImporter[ext] = importer;
		}

		AssetImporter* GetImporter(const fs::path& extension)const
		{
			auto it = m_extToImporter.find(extension);
			if (it != m_extToImporter.end())
				return it->second.get();
			return nullptr;
		}

		const std::vector<AssetImporter*>& GetImporters()
		{
			static std::vector<AssetImporter*> importers;

			if (importers.empty())
				for (auto& imp : m_extToImporter)
					importers.emplace_back(imp.second.get());

			return importers;
		}
	private:
		std::unordered_map<fs::path, SRef<AssetImporter>> m_extToImporter; //TODO: maybe switch to string for ext

	private:
		AssetImporterRegistry() = default;

	public:
		AssetImporterRegistry(const AssetImporterRegistry& other) = delete;
		AssetImporterRegistry(AssetImporterRegistry&& other) = delete;
		AssetImporterRegistry& operator=(const AssetImporterRegistry& other) = delete;
		AssetImporterRegistry& operator=(AssetImporterRegistry&& other) = delete;

	};
}

#define REGISTER_IMPORTER(type)        \
namespace Registry                  \
{                                          \
    struct type##Register                  \
    {                                      \
        type##Register()                      \
        {                                     \
            Twisted::AssetImporterRegistry::GetInstance().RegisterImporter<Twisted::type>(); \
        }                                     \
    };                                        \
    static type##Register s_##type##Register; \
}


#pragma once
#include "Rendering/Texture.h"

#include <unordered_map>
#include <memory>
#include <filesystem>
#include <vector>

namespace Twisted
{
	class IconDatabase
	{
	public:
		std::shared_ptr<Render::Texture> GetPreviewTexture(const fs::path& path);
		std::shared_ptr<Render::Texture> GetIconTexture(const fs::path& path);
		void Clear();

	private:
		std::unordered_map<fs::path, std::shared_ptr<Render::Texture>> m_previewTextures;
		std::unordered_map<fs::path, std::shared_ptr<Render::Texture>> m_iconTextures;

		std::vector<int> m_iconIndices;
		std::vector<std::string> m_iconFilepaths; // m_iconIndices[x] <-> m_iconFilepaths[x]

	};
}

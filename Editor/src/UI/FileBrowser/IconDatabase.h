#pragma once
#include "editorpch.h"
#include "Assets/TextureData.h"
#include "Twisted/Rendering/Texture.h"

namespace Twisted
{
	class IconDatabase
	{
	public:
		std::shared_ptr<Texture> GetPreviewTexture(const fs::path& path);
		std::shared_ptr<Texture> GetIconTexture(const fs::path& path);
		void Clear();

	private:
		std::unordered_map<fs::path, std::shared_ptr<Texture>> m_previewTextures;
		std::unordered_map<fs::path, std::shared_ptr<Texture>> m_iconTextures;

		std::vector<int> m_iconIndices;
		std::vector<std::string> m_iconFilepaths; // m_iconIndices[x] <-> m_iconFilepaths[x]

	};
}
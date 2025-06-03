#include "editorpch.h"
#include "IconDatabase.h"
#include "Rendering/RenderingAPI.h"
#include "Twisted.h"

namespace Twisted
{
	std::shared_ptr<Render::Texture> IconDatabase::GetPreviewTexture(const fs::path& path)
	{
		auto it = m_previewTextures.find(path);
		if (it != m_previewTextures.end())
		{
			if (!it->second)
				return GetIconTexture(path);
			return it->second;
		}

		std::shared_ptr<Render::TextureData> texData = ImportTextureData(path);
		if (!texData)
		{
			m_previewTextures[path] = nullptr;
			return nullptr;
		}

		std::shared_ptr<Render::Texture> tex = nullptr;// Texture::CreateTexture(*texData);
		m_previewTextures[path] = tex;
		return tex;
	}
	std::shared_ptr<Render::Texture> IconDatabase::GetIconTexture(const fs::path& path)
	{
		if (m_iconTextures.count(path.string()) > 0)
			return m_iconTextures[path.string()];

		std::string pathString = path.string();

		std::error_code ec;
		m_iconTextures[pathString] = nullptr;

		DWORD attrs = 0;
		UINT flags = SHGFI_ICON | SHGFI_LARGEICON;
		if (!std::filesystem::exists(path, ec)) {
			flags |= SHGFI_USEFILEATTRIBUTES;
			attrs = FILE_ATTRIBUTE_DIRECTORY;
		}

		SHFILEINFOW fileInfo = { 0 };
		std::wstring pathW = path.wstring();
		for (int i = 0; i < pathW.size(); i++)
			if (pathW[i] == '/')
				pathW[i] = '\\';
		SHGetFileInfoW(pathW.c_str(), attrs, &fileInfo, sizeof(SHFILEINFOW), flags);

		if (fileInfo.hIcon == nullptr)
			return nullptr;

		// check if icon is already loaded
		auto itr = std::find(m_iconIndices.begin(), m_iconIndices.end(), fileInfo.iIcon);
		if (itr != m_iconIndices.end()) {
			const std::string& existingIconFilepath = m_iconFilepaths[itr - m_iconIndices.begin()];
			m_iconTextures[pathString] = m_iconTextures[existingIconFilepath];
			return m_iconTextures[pathString];
		}

		m_iconIndices.push_back(fileInfo.iIcon);
		m_iconFilepaths.push_back(pathString);

		ICONINFO iconInfo = { 0 };
		GetIconInfo(fileInfo.hIcon, &iconInfo);

		if (iconInfo.hbmColor == nullptr)
			return nullptr;

		DIBSECTION ds;
		GetObject(iconInfo.hbmColor, sizeof(ds), &ds);
		int byteSize = ds.dsBm.bmWidth * ds.dsBm.bmHeight * (ds.dsBm.bmBitsPixel / 8);

		if (byteSize == 0)
			return nullptr;

		uint8_t* data = (uint8_t*)malloc(byteSize);
		GetBitmapBits(iconInfo.hbmColor, byteSize, data);

		std::shared_ptr<Render::TextureData> texData = std::make_shared<Render::TextureData>(ds.dsBm.bmWidth, ds.dsBm.bmHeight, 4, data);
		std::shared_ptr<Render::Texture> tex = nullptr;// Texture::CreateTexture(*texData);
		m_iconTextures[path] = nullptr;// Texture::CreateTexture(*texData);
		return m_iconTextures[pathString];
	}

	void IconDatabase::Clear()
	{
		m_iconTextures.clear();
		m_previewTextures.clear();
		m_iconFilepaths.clear();
		m_iconIndices.clear();
		//TODO:....
	}

}
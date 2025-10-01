#pragma once
#include "UI/EditorPanel.h"
#include <filesystem>
#include "EditorLayer.h"
#include "Twisted/Data/Project.h"
#include "Twisted/RegisterLayer/ObjectManager.h"
#include "UI/ImguiExtensions.h"
#include <imgui.h>
#include <optional>

namespace fs = std::filesystem;

namespace Twisted::Editor
{
	class AssetsPanel :public EditorPanel
	{
	public:
		const float ICON_SIZE = 64.0f;
		const float PADDING = 16.0f;
		const ImVec2 CELL_SIZE{ ICON_SIZE + PADDING,ICON_SIZE + PADDING };
		const ImVec4 HIGHLIGHT_COLOR{ 0.4f, 0.4f, 1.0f, 1.0f };

		AssetsPanel(EditorLayer* editor);
		void PaintContent()override;

	private:

		void NewAssetPopup();
		void PaintNewAsset();

		void PaintTreePart(const fs::path& dirPath);

		fs::path currentDir;

		std::optional<std::filesystem::path> m_selectedPath = std::nullopt;
		std::optional <Im::InputTextToken> m_newFileName = std::nullopt;
	};
}
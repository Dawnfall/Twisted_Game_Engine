#include "DetailsPanel.h"

#include "EditorLayer.h"

#include "Selection.h"
#include "Twisted/Gameing/World.h"
#include "EditorRegistry.h"
#include <variant>
#include <type_traits>
#include "UI/ImguiExtensions.h"
#include "UI/ComponentPainter.h"

namespace Twisted::Editor
{
	void DetailsPanel::PaintContent()
	{
		std::visit([this](auto&& arg)
			{
				using SelectionType = std::decay_t<decltype(arg)>;
				if constexpr (std::is_base_of_v<SelectionType, std::unordered_set<EntityID>>)
				{
					if (arg.size() == 1)
					{
						World* gameWorld = m_editor->GetGameWorld();
						//EditorRegistry::GetInstance().GetDetailsPainter<EntityPainter>();
					}
				}
				//else if constexpr (std::is_base_of_v<AssetInfo, T>)
				//{
				//	AssetPainter assetPainter;
				//	assetPainter.Paint(arg);
				//}
				//else if constexpr (std::is_same_v<std::monostate, T>)
				//{

				//}
			}, Selection::GetInstance().GetSelection());
	}

	
}
#include "DetailsPanel.h"

#include "EditorLayer.h"

#include "Twisted/Gameing/World.h"
#include "EditorRegistry.h"
#include <variant>
#include <type_traits>
#include "UI/ImguiExtensions.h"
#include "UI/ComponentPainter.h"

#include "UI/Details/EntityPainter.h"

#include "EditorMacros.h"
#include "EditorData/EditorData.h"


namespace Twisted::Editor
{
	void DetailsPanel::PaintContent()
	{
		//std::visit([this](auto&& arg)
		//	{
		//		using SelectionType = std::decay_t<decltype(arg)>;
		//		if constexpr (is_unordered_set<SelectionType>::value)
		//		{
		//			if (arg.size() == 1)
		//			{
		//				Entity entity = *arg.begin();
		//				EntityPainter* entPainter=EditorRegistry::GetInstance().GetDetailsPainter<EntityPainter>();
		//				if (entPainter)
		//					entPainter->Paint(&entity);
		//			}
		//		}
		//		//else if constexpr (std::is_base_of_v<AssetInfo, T>)
		//		//{
		//		//	AssetPainter assetPainter;
		//		//	assetPainter.Paint(arg);
		//		//}
		//		//else if constexpr (std::is_same_v<std::monostate, T>)
		//		//{

		//		//}
		//	}, EditorData::GetInstance().GetSelection().GetSelection());	
	}


}

REGISTER_EDITOR_PANEL(DetailsPanel)
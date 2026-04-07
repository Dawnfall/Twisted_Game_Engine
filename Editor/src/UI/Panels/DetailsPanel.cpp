#include "DetailsPanel.h"

#include "Twisted/Gameing/World.h"

#include "EditorApp/EditorService.h"
#include "EditorApp/EditorRegistry.h"
#include "UI/ImguiExtensions.h"
#include "UI/ComponentPainter.h"
#include "UI/Details/EntityPainter.h"

#include <variant>
#include <type_traits>


namespace Twisted::Editor
{
	void DetailsPanel::PaintContent()
	{
		auto& selection = Application::GetInstance().GetService<EditorService>()->GetSelection();

		switch (selection.GetActiveType())
		{
		case SelectionType::ENTITY:
		{
			const std::unordered_set<Entity>& selectedEntities = selection.GetSelectedEntities();
			if (selectedEntities.size() >= 1)
			{
				Entity entity = *selectedEntities.begin();
				DetailsPainter* entPainter = EditorRegistry::GetInstance().GetDetailsPainter(std::type_index(typeid(entity)));
				if (entPainter)
					entPainter->Paint(&entity);
			}
			break;
		}
		case SelectionType::ASSET:
		{
			auto& selectedPaths = selection.GetSelectedPaths();
			if (selectedPaths.size() >= 1)
			{
				auto assetsLayer = Application::GetInstance().GetService<AssetsService>();
				auto info = assetsLayer->GetInfo(*selectedPaths.begin());
				auto& objects = assetsLayer->GetManagedAssetObjects(info);

				if (objects.size() >= 1)
				{
					TObject* obj = objects.begin()->GetObj();
					DetailsPainter* assetPainter = EditorRegistry::GetInstance().GetDetailsPainter(std::type_index(typeid(*obj)));
					if (assetPainter)
						assetPainter->Paint(obj);
				}
			}
			break;
		}
		default:
			break;
		}
	}


}

REGISTER_EDITOR_PANEL(DetailsPanel)

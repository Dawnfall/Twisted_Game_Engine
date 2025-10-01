#include "EditorRuntime.h"
#include "EditorConstants.h"

#include "Twisted/Application/Application.h"

#include "EditorRegistry.h"
#include "EditorLayer.h"
#include "ProjectLoader.h"
#include "Twisted/Rendering/RenderLayer.h"
#include "Twisted/Windowing/WindowLayer.h"
#include "Twisted/RegisterLayer/ObjectManager.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"

#include "UI/Panels/DetailsPanel.h"
#include "UI/Panels/TreeViewPanel.h"
#include "UI/Panels/WorldViewPanel.h"
#include "UI/Panels/AssetsPanel.h"
#include "UI/ImguiExtensions.h"

#include "UI/Details/Components/CTransformPainter.h"
#include "UI/Details/Components/CNamePainter.h"
#include "UI/Details/Components/CRendererPainter.h"

#include "Twisted/AssetsLayer/Importers/TextureImporter.h"
#include "Twisted/AssetsLayer/Importers/ShaderImporter.h"
#include "Twisted/AssetsLayer/Importers/ModelImporter.h"
#include "Twisted/AssetsLayer/Importers/WorldImporter.h"
#include "Twisted/AssetsLayer/Importers/MaterialImporter.h"

#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Shader.h"
#include "Twisted/BuiltIn/MeshCollections.h"

#include <filesystem>
#include <string>
#include <memory>
#include <imgui.h>

namespace Twisted::Editor
{
	void EditorRuntime::OnCreate()
	{
		RegisterLayers();
	}

	void EditorRuntime::LoadBuiltIn()
	{
		ObjectManager::GetInstance().CreateObject<Mesh>(Collections::triangleMesh);
		ObjectManager::GetInstance().CreateObject<Mesh>(Collections::quadMesh);
		ObjectManager::GetInstance().CreateObject<Mesh>(Collections::cubeMesh);
	}

	void EditorRuntime::OnBeforeRun()
	{
		m_editorLayer->EditorConfigData.LoadConfig();

		RegisterImporters();
		RegisterPanels();
		RegisterPainters();

		Project::GetInstance().ProjectChangeEvent.AddListener([this]() {
			layoutFilePath = (Project::GetInstance().GetRootPath() / "EditorLayout.ini").string();
			Im::SetLayoutIniFile(layoutFilePath,true);

			m_window->SetTitle(Constants::EDITOR_WINDOW_TITLE + " " + Project::GetInstance().GetName());
			m_window->SetSize(m_editorLayer->EditorConfigData.GetWindowSize());
			m_window->SetPosition(m_editorLayer->EditorConfigData.GetWindowPos());
			LoadResources();
			});

		StartWindow();

		m_window->SetSize(Vec2i{ 1200,500 });
	}

	void EditorRuntime::OnFrame()
	{
		m_window->PollEvents();
		m_window->Clear(Constants::WINDOW_CLEAR_COLOR);

		Im::StartFrame();
		if (Project::GetInstance().GetRootPath() == "")
		{
			m_projectLoader->Render(m_window);
		}
		else
		{
			m_editorLayer->Render(m_window);
		}
		Im::EndFrame();

		m_window->SwapBuffers();
	}

	void EditorRuntime::OnTerminate()
	{
		m_editorLayer->EditorConfigData.SetWindowPos(m_window->GetPosition());
		m_editorLayer->EditorConfigData.SetWindowSize(m_window->GetSize());

		m_editorLayer->EditorConfigData.SaveConfig();
	}

	void EditorRuntime::StartWindow()
	{
		m_window = m_windowLayer->CreateNewWindow(Constants::LOADUP_WIN_TITLE,Constants::LOADUP_WIN_SIZE,Constants::LOADUP_WIN_POS);
		InitRenderer(m_window->GetContextAdress());
		Im::Init(m_windowLayer->GetWindow()->GetRawPointer());
		Im::SetLayoutIniFile("");

		m_window->CloseWindowEvent.AddListener([&]() {
			this->App->Stop();
			});
		m_window->SetTitle(Constants::EDITOR_WINDOW_TITLE);
	}

	void EditorRuntime::LoadResources()
	{
		auto assetsFolder = Project::GetInstance().GetAssetsFolder();
		m_assetsLayer->ImportAssets(assetsFolder);
		LoadBuiltIn();
	}

	void EditorRuntime::RegisterLayers()
	{
		m_projectLoader = App->AddLayer<ProjectLoader>();

		m_windowLayer = App->AddLayer<Twisted::WindowLayer>();
		m_renderLayer = App->AddLayer<Twisted::RenderLayer>();
		m_assetsLayer = App->AddLayer<AssetsLayer>();
		m_editorLayer = App->AddLayer<EditorLayer>();
	}

	void EditorRuntime::RegisterPanels()
	{
		m_editorLayer->CreateEditorPanel<TreeViewPanel>();
		m_editorLayer->CreateEditorPanel<DetailsPanel>();
		m_editorLayer->CreateEditorPanel<WorldViewPanel>();
		m_editorLayer->CreateEditorPanel<AssetsPanel>();
	}

	void EditorRuntime::RegisterImporters()
	{
		m_assetsLayer->RegisterImporter<Twisted::TextureImporter>();
		m_assetsLayer->RegisterImporter<Twisted::ShaderImporter>();
		m_assetsLayer->RegisterImporter<Twisted::ModelImporter>();
		m_assetsLayer->RegisterImporter<Twisted::WorldImporter>();
		m_assetsLayer->RegisterImporter<Twisted::MaterialImporter>();
	}

	void EditorRuntime::RegisterPainters()
	{
		EditorRegistry::GetInstance().RegisterComponentPainter<CNamePainter>();
		EditorRegistry::GetInstance().RegisterComponentPainter<CTransformPainter>();
		EditorRegistry::GetInstance().RegisterComponentPainter<CRendererPainter>();
	}

	void EditorRuntime::CreateObjects()
	{
		//for (const auto& pair : m_assetsLayer->GetAssets())
		//{
		//	pair.second->LoadCreate(m_registerLayer);
		//}

		//TODO:... post Load...assign pointers from uuids
		//TODO.... create temp object that hold pointer to new object and other data and can call postload()
	}
}
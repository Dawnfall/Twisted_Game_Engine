#pragma once
#include "Twisted/Application/Application.h"

#include "ProjectLoader/ProjectLoader.h"
#include "EditorLayer.h"
#include "Twisted/Rendering/RenderLayer.h"
#include "Twisted/Windowing/WindowLayer.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"

#include "EditorConstants.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Rendering/RenderingAPI.h"
#include "BuiltIn/MeshCollections.h"

std::string layoutFilePath = "";

namespace Twisted
{
	class Application;
	class WindowLayer;
	class RenderLayer;
	class AssetsLayer;
	class Window;
}

namespace Twisted::Editor
{
	class EditorApplication : public Application
	{
	public:
		EditorApplication() = default;

	private:
		void OnCreate() override
		{
			RegisterLayers();
		}

		void OnBeforeRun() override
		{
			m_editorLayer->EditorConfigData.LoadConfig();
			Project::GetInstance().ProjectChangeEvent.AddListener([this]() {
				layoutFilePath = (Project::GetInstance().GetRootPath() / "EditorLayout.ini").string();
				Im::SetLayoutIniFile(layoutFilePath, true);

				m_window->SetTitle(Constants::EDITOR_WINDOW_TITLE + " " + Project::GetInstance().GetName());
				m_window->SetSize(m_editorLayer->EditorConfigData.GetWindowSize());
				m_window->SetPosition(m_editorLayer->EditorConfigData.GetWindowPos());
				LoadResources();
				});

			StartWindow();

			LoadBuiltIn();
			m_window->SetSize(Vec2i{ 1200,500 });
		}

		void OnFrame() override
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

		void OnTerminate()override
		{
			m_editorLayer->EditorConfigData.SetWindowPos(m_window->GetPosition());
			m_editorLayer->EditorConfigData.SetWindowSize(m_window->GetSize());

			m_editorLayer->EditorConfigData.SaveConfig();
		}

	private:
		void LoadBuiltIn()
		{
			TObject::Create<Mesh>(Collections::triangleMesh, MeshParams{});
			TObject::Create<Mesh>(Collections::quadMesh, MeshParams{});
			TObject::Create<Mesh>(Collections::cubeMesh, MeshParams{});
		}

		void StartWindow()
		{
			m_window = m_windowLayer->CreateNewWindow(Constants::LOADUP_WIN_TITLE, Constants::LOADUP_WIN_SIZE, Constants::LOADUP_WIN_POS);
			InitRenderer(m_window->GetContextAdress());
			Im::Init(m_windowLayer->GetWindow()->GetRawPointer());
			Im::SetLayoutIniFile("");

			m_window->CloseWindowEvent.AddListener([&]() {
				Stop();
				});
			m_window->SetTitle(Constants::EDITOR_WINDOW_TITLE);
		}

		void LoadResources()
		{
			auto assetsFolder = Project::GetInstance().GetAssetsFolder();
			m_assetsLayer->ImportAssets(assetsFolder);
			LoadBuiltIn();
		}

		void RegisterLayers()
		{
			m_projectLoader = AddLayer<ProjectLoader>();

			m_windowLayer = AddLayer<WindowLayer>();
			m_renderLayer = AddLayer<RenderLayer>();
			m_assetsLayer = AddLayer<AssetsLayer>();
			m_editorLayer = AddLayer<EditorLayer>();
		}

		void Create()
		{
			//for (const auto& pair : m_assetsLayer->GetAssets())
			//{
			//	pair.second->LoadCreate(m_registerLayer);
			//}

			//TODO:... post Load...assign pointers from uuids
			//TODO.... create temp object that hold pointer to new object and other data and can call postload()
		}

	private:
		Window* m_window = nullptr;

		WindowLayer* m_windowLayer = nullptr;
		RenderLayer* m_renderLayer = nullptr;
		AssetsLayer* m_assetsLayer = nullptr;
		EditorLayer* m_editorLayer = nullptr;
		ProjectLoader* m_projectLoader = nullptr;
	};
}